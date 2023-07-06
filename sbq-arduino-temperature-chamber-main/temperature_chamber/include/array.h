#ifndef ARRAY_H
#define ARRAY_H

template<class ValueType>
class Array {
public:
    Array(const ValueType* input_values, int size): size(size) {
        values = new ValueType[size];
        for(int i=0; i<size; ++i) values[i] = input_values[i];
    }

    Array(const Array& other): size(other.size) {
        values = new ValueType[size];
        for(int i=0; i<size; ++i) values[i] = other.values[i];
    }

    Array& operator=(const Array& other) {
        if (this == &other) {
            return *this;
        }
        size = other.size;
        delete[] values;
        values = new ValueType[size];
        for(int i=0; i<size; ++i) values[i] = other[i];
        return *this;
    }

    ~Array() {
        delete[] values;
    }

    ValueType operator[](int index) const {
        return values[index];
    }

    int size;

private:
    ValueType* values;
};

template<class InputType, class OutputType, class Callable>
Array<OutputType> map(const Callable& function, const Array<InputType>& inputs) {
    OutputType values[inputs.size];
    for (int i=0; i<inputs.size; ++i) values[i] = function(inputs[i]);
    return Array<OutputType>(values, inputs.size);
}

#endif /* ARRAY_H */
