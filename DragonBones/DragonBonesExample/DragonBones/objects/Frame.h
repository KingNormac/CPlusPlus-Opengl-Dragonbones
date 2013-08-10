#ifndef FRAME_H
#define FRAME_H

#include <string>

namespace DragonBones {

class Frame {
public:
	float position;
	float duration;
	std::string action;
	std::string event;
	std::string sound;

	Frame(){
	    position = 0;
		duration = 0;
	}

	~Frame(){}

};
};


#endif