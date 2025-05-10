#pragma once

class carte {
    private:
        int _value;
        bool _faceUp;
        bool _delete;

    public:
        carte(int const & value);
        carte(const carte& other)
        : _value(other._value), _faceUp(other._faceUp), _delete(other._delete) {
    }
     carte() : _value(0), _faceUp(false), _delete(false) {}

        int get_value() const { return _value; }
        bool get_faceUp() const { return _faceUp; }
        bool get_delete() const { return _delete; }

        void set_faceUp(bool const & faceUp) { _faceUp = faceUp; }
        void set_delete(bool const & delete_) { _delete = delete_; }
};
