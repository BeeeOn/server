#ifndef BEEEON_UI_SERIALIZING_H
#define BEEEON_UI_SERIALIZING_H

namespace BeeeOn {

class Place;
class Gateway;

namespace UI {

std::string serialize(const Place &p);
std::ostream &serialize(std::ostream &o, const Place &place);
std::istream &deserialize(std::istream &i, Place &place);

std::string serialize(const Gateway &g);
std::ostream &serialize(std::ostream &o, const Gateway &gateway);
std::istream &deserialize(std::istream &i, Gateway &gateway);


}
}

#endif
