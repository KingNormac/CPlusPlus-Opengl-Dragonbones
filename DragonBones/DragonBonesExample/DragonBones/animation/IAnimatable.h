#ifndef IANIMATABLE_H
#define IANIMATABLE_H

namespace DragonBones {
	class IAnimatable {
	public:
		virtual void advanceTime(float passedTime){}
	};
}


#endif