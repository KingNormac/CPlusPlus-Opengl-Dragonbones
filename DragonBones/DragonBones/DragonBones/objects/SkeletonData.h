#ifndef SKELETONDATA_H
#define SKELETONDATA_H

#include "ArmatureData.h"
#include <string>
#include <vector>
#include <map>
#include "../as3/Vector2D.h"

namespace DragonBones {

	class SkeletonData {
	private:
		std::map<std::string, Vector2D*> _subTexturePivots;
		std::vector<ArmatureData*> _armatureDataList;
	public:
		std::string name;

		std::vector<std::string> getArmatureNames();
		std::vector<ArmatureData*> getArmatureDataList();

		SkeletonData();
		~SkeletonData();

		ArmatureData* getArmatureData(std::string armatureName);
		void addArmatureData(ArmatureData* armatureData);
		void removeArmatureData(ArmatureData* armatureData);
		void removeArmatureDataByName(std::string armatureName);

		Vector2D* getSubTexturePivot(std::string subTextureName);
		Vector2D* addSubTexturePivot(float x, float y, std::string name);
		void removeSubTexturePivot(std::string subTextureName);
		
	};

};

#endif