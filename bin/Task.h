#ifndef SDP_PROJECT_TASK_H
#define SDP_PROJECT_TASK_H

class Task {
public:
    [[nodiscard]] virtual bool isStopTask() const = 0;
};

class AlternationTask : public Task {
private:
    int category1;
    int category2;
    bool stop_signal;
public:
    explicit AlternationTask(bool stop_signal);

    AlternationTask(int category1, int category2);

    [[nodiscard]] bool isStopTask() const override;

    const int &getCategory1() const;
    const int &getCategory2() const;
    bool to_be_alternated() const;
};




#endif //SDP_PROJECT_TASK_H
