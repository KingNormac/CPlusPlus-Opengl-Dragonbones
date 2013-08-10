#ifndef TIMELINE_H
#define TIMELINE_H

#include <vector>
#include "Frame.h"
#include "../as3/Object.h"

namespace DragonBones {

class Timeline : public Object{
public:
	std::vector<Frame*> _frameList;
	float _duration;
	float _scale;

	Timeline();
	~Timeline();
	
	std::vector<Frame*> getFrameList();
	void addFrame(Frame* frame);	

	float getDuration();	
	void setDuration(float value);

	float getScale();
	void setScale(float value);	
	
};

};

#endif

