#ifndef PID_H
#define PID_H

class Pid {
public:
    Pid(float, float, float);
    void compute_pid_term(float);
    float compute_pid_command();
    void set_kp(float);
    void set_kd(float);
    void set_ki(float);
    float get_kp();
    float get_kd();
    float get_ki();
    float get_proportional();
    float get_derivative();
    float get_integral();
private:
    float time_interval();
    float mean_errors(const float *);
    void permutate_errors(float *, float);

    float kp;
    float kd;
    float ki;
    float proportional;
    float derivative;
    float integral;
};

#endif /* PID_H */
