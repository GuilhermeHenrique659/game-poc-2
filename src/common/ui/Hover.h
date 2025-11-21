#include "../Subject.h"

class OnHover : public Observer
{
private:
    /* data */
public:
    OnHover() = default;
    ~OnHover() = default;
    void notify(const std::any &data) override;
};