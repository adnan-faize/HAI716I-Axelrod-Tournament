#include <string>

/**
 * @brief Abstract class
 */
class Strategy {
    public:
        virtual ~Strategy() = default;

        virtual std::string name() const = 0;
        virtual std::string description() const = 0;
        virtual std::string expected_behavior() const = 0;

        virtual void reset() {}
};
