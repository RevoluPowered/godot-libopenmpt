#ifndef OPENMPT_PLAYER_H
#define OPENMPT_PLAYER_H

#include <godot_cpp/classes/node.hpp>

namespace godot {

class OpenMPTPlayer : public Node {
    GDCLASS(OpenMPTPlayer, Node)

private:
    double time_passed;

protected:
    static void _bind_methods();

public:
    OpenMPTPlayer();
    ~OpenMPTPlayer();

    void _process(double delta) override;

    void set_amplitude(const double p_amplitude);
    double get_amplitude() const;
};

}

#endif // OPENMPT_PLAYER_H
