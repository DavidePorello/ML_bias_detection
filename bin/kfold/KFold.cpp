#include <numeric>
#include <iostream>
#include "KFold.h"

/**
 * Class in charge of computing the K-fold predictions on a dataset, using num_threads inside a thread pool
 * @param num_folds: number of folds for the K-folds
 * @param num_threads: number of threads to use for the parallel computation
 * @param labels: vector of the label values (wage, e.g. 1020.50)
 * @param model: ML model class
 * @param num_categories: number of categories of the alternated attribute (e.g. attribute gender has 2 categories -> male, female)
 * @param attribute_index: index of the attribute (i.e. in the dataset, the values for the alternated attribute are on column <attribute_index>)
 */
KFold::KFold(int num_folds, int num_threads,
             const Eigen::VectorXf &labels, ModelML &model,
             int num_categories, int attribute_index) : num_folds(num_folds),
                                                        num_threads(num_threads),
                                                        labels(labels),
                                                        model(model),
                                                        num_categories(num_categories),
                                                        attribute_index(attribute_index) {
    // create thread pool
    function<void(KFoldTask &&)> kfold_func = [this](KFoldTask &&t) -> void {
        this->run_model(t);
    };
    this->pool = make_unique<ThreadPool<KFoldTask>>(kfold_func, this->num_threads, false);
}

/**
 * Wrapper function which just waits for the dataset to be computed, then runs KFold::compute_predictions()
 * @param dataset: a future containing the dataset to compute the predictions on.
 * @return a future containing the result of KFold::compute_predictions()
 */
future<vector<future<Eigen::MatrixXf>>> KFold::compute_predictions_async_pool(future<AlternatedMatrix> &dataset) {
    future<vector<future<Eigen::MatrixXf>>> fut = async(launch::async,
                                                         [this, &dataset]() -> vector<future<Eigen::MatrixXf>> {
                                                             AlternatedMatrix d = dataset.get();
                                                             return compute_predictions(d);
                                                         });
    return move(fut);
}

/**
 * Insert NUM_FOLDS tasks in the thread pool, each running run_model()
 * @param dataset: the dataset on which to run the ML model
 * @return a vector of futures, each containing the processed predictions as a matrix of shape (NUM_CATEGORIES x 2):
 *      <ul>
 *          <li> Each row is relative to 1 categorical value (male, female).
 *          <li> Column 0 contains the means of the predictions for that category,
 *          <li> Column 1 contains their standard deviation
 *      </ul>
 */
vector<future<Eigen::MatrixXf>> KFold::compute_predictions(const AlternatedMatrix &dataset) {

    vector<future<Eigen::MatrixXf>> predictions;

    // train and predict on the alternated dataset
    for (int i = 0; i < num_folds; i++) {
        KFoldTask task(dataset, i);
        predictions.emplace_back(task.get_future());
        pool->enqueue(move(task));
    }

    return predictions;
}

/**
 * Extract the test fold from the dataset, then run the ML model to get a set of predictions.
 * The predictions are processed and returned inside t.promise_predictions as a matrix of shape (NUM_CATEGORIES x 2):
 *      <ul>
 *          <li> Each row is relative to 1 categorical value (male, female).
 *          <li> Column 0 contains the means of the predictions for that category,
 *          <li> Column 1 contains their standard deviation
 *      </ul>
 * @param t: KFoldTask containing the index of the fold to process
 */
void KFold::run_model(KFoldTask &t) {
    int i, j;
    Eigen::VectorXf predictions;
    Eigen::MatrixXf test;
    Eigen::MatrixXf &dataset = t.getDataset().dataset;
    int num_records = dataset.rows();
    int num_cols = dataset.cols();
    int test_fold_index = t.get_test_fold_index();
    int test_fold_start = get_fold_start_index(num_records, test_fold_index);
    int test_fold_end = get_fold_start_index(num_records, test_fold_index + 1);
    int test_fold_size = test_fold_end - test_fold_start;
    int num_train_records = num_records - test_fold_size;

    // pop test fold from dataset
    test = dataset.block(test_fold_start, 0, test_fold_size, num_cols);
    Eigen::MatrixXf data(num_train_records, num_cols);
    data.block(0, 0, test_fold_start, num_cols) = dataset.block(0, 0, test_fold_start, num_cols);
    data.block(test_fold_start, 0, num_records - test_fold_end, num_cols) = dataset.block(test_fold_end, 0,
                                                                                          num_records - test_fold_end,
                                                                                          num_cols);
    Eigen::VectorXf data_labels(num_train_records);
    data_labels.segment(0, test_fold_start) = labels.segment(0, test_fold_start);
    data_labels.segment(test_fold_start, num_records - test_fold_end) = labels.segment(test_fold_end,
                                                                                       num_records - test_fold_end);

    // train the model
    model.fit(data, data_labels);
    // predict the values
    model.predict(test, predictions);

    // extract predictions relative to all categories and compute their means
    Eigen::MatrixXf results = move(process_results(test, predictions, t.getDataset().a1, t.getDataset().a2));

    //TODO remove
    if (test_fold_index == 4) {
        //cout << predictions << endl;
        cout << results << endl;
    }

    t.set_predictions(move(results));
}

/**
 * get the index of the first record of a fold. It uses a mathematical formula, so it doesn't throw Segmentation Fault if the index falls outside the dataset
 * @param num_records: number of records in the dataset
 * @param fold_index: index of the fold, starting from zero
 * @return index of the first record of a fold
 */
int KFold::get_fold_start_index(int num_records, int fold_index) const {
    return fold_index * (num_records / num_folds) + min(fold_index, num_records % num_folds);
}


/**
 * Function to process the predictions and condense them to just the means and standard deviation for each category
 * @param test: the test fold. Each row is a record, each column an attribute
 * @param predictions: the prediction for each record in the test fold (in the same order)
 * @return matrix of shape (NUM_CATEGORIES x 2):
 *      <ul>
 *          <li> Each row is relative to 1 categorical value (male, female).
 *          <li> Column 0 contains the means of the predictions for that category,
 *          <li> Column 1 contains their standard deviation
 *      </ul>
 */
Eigen::MatrixXf
KFold::process_results(const Eigen::MatrixXf &test, const Eigen::VectorXf &predictions, int a1, int a2) const {
    int i;
    vector<vector<float>> cat_preds(
            num_categories); // vector. each element is a vector containing all predictions relative to 1 category
    for (i = 0; i < test.rows(); i++) {
        int category = static_cast<int>(round(test(i, attribute_index)));
        cat_preds[category].emplace_back(predictions(i));
    }

    Eigen::MatrixXf results(num_categories, 2);
    for (i = 0; i < num_categories; i++) {
        // convert vector of predictions for category i in a Eigen Vector
        Eigen::Map<Eigen::VectorXf> preds(cat_preds[i].data(), cat_preds[i].size());

        // if we have predictions for this category, compute their mean and standard deviation. (else set them to zero)
        if (preds.size() > 0) {
            float mean = preds.mean();
            results(i, 0) = mean;
            results(i, 1) = stddev(preds, mean);
        } else
            results(i, 0) = results(i, 0) = 0;
    }

    // If the dataset is alternated, swap the rows relative to the swapped attributes
    if(a1>=0 && a2>=0) {
        Eigen::VectorXf tmp = results.row(a1);
        results.row(a1) = results.row(a2);
        results.row(a2) = tmp;
    }

    return move(results);
}

/**
 * Join all threads spawned by this object
 */
void KFold::join_threads() {
    // signal the pool to stop
    KFoldTask stop_task(true);
    pool->enqueue(move(stop_task));

    //join threads
    pool->join_threads();
}

/** Utility function to compute the standard deviation in place.
 *  @param p the array we want to compute the standard deviation
 *  @param mean the mean of p
 *  @param unbiased if true, we divide by N-1 instead of N to reduce bias
 *  (https://en.wikipedia.org/wiki/Unbiased_estimation_of_standard_deviation)
 *  */
float stddev(const Eigen::VectorXf &p, float mean, bool unbiased) {
    float diff;
    float sum = 0;
    for (float i : p) {
        // compute difference
        diff = i - mean;
        sum += diff * diff;
    }
    if (unbiased) {
        // divide by N - 1
        sum /= static_cast<float>(p.size()) - 1;
    } else {
        // divide by N
        sum /= static_cast<float>(p.size());
    }
    // return square root
    return std::sqrt(sum);
}
