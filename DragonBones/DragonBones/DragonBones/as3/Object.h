#ifndef OBJECT_H
#define OBJECT_H
/**
*This class is the eqivulant of AS3's object class
*It allows us to put any class that inherits it inside the same
*vector
**/

#include <string>

namespace DragonBones {
	class Object {
	public:
		Object(std::string value){parentClass = value;}
		~Object(){}
		std::string parentClass;
		virtual std::string toString(){return "Object: toString() not overwriten \n";}
	};
}


template <class T>
	bool checkIfClass(DragonBones::Object* object){
		if(T* check = dynamic_cast< T* >( object ) )
		if(check) {
			return true;
		}
		 return false;
	}

#endif