#ifndef WORLDCLOCK_H
#define WORLDCLOCK_H

#include "GLFW\glfw3.h"
#include <vector>
#include "IAnimatable.h"

namespace DragonBones {
	#define WorldClock Clock::Instance()

	class Clock {

	private:
		Clock();
		double _time;
		std::vector<IAnimatable*> animatableList;

	public:
		static Clock* Instance();
		float	timeScale;

		double	getTime();
		bool	contains(IAnimatable* animatable);
		void	add(IAnimatable* animatable);
		void	remove(IAnimatable* animatable);
		void	clear();
		void	advanceTime(float passedTime);

		double getTimer() {
			return glfwGetTime() * 1000;//GLFW returns in second while as3 returns in milliseconds. 
		}
	};
}
#endif