#include "SkeletonData.h"

namespace DragonBones {

	std::vector<std::string> SkeletonData::getArmatureNames() {
		std::vector<std::string> names;
		for(int i=0;i<_armatureDataList.size();i++) {
			ArmatureData* data = _armatureDataList[i];
			if(data) {
				names.push_back(data->name);
			}
		}
		return names;
	}

	std::vector<ArmatureData*> SkeletonData::getArmatureDataList() {
		return this->_armatureDataList;
	}

	SkeletonData::SkeletonData() {
		_armatureDataList.clear();
		_subTexturePivots.clear();
	}

	SkeletonData::~SkeletonData() {
		for(int i=0;i<_armatureDataList.size();i++) {
			ArmatureData* data = _armatureDataList[i];
			if(data) {
				delete[] data;
			}
		}
		_armatureDataList.clear();
	}

	ArmatureData* SkeletonData::getArmatureData(std::string armatureName) {
		for(int i=0;i<_armatureDataList.size();i++) {
			ArmatureData* data = _armatureDataList[i];
			if(data) {
				if(data->name == armatureName) {
					return data;
				}
			}
		}
		return 0;
	}

	void SkeletonData::addArmatureData(ArmatureData* armatureData) {
		if(armatureData) {
			_armatureDataList.push_back(armatureData);
		} else {
			printf("Tried to add bad armaturedata to skeletondata \n");
		}
	}

	void SkeletonData::removeArmatureData(ArmatureData* armatureData) {
		if(armatureData) {
			for(int i=0;i<_armatureDataList.size();i++) {
				ArmatureData* match = _armatureDataList[i];
				if(match) {
					if(match == armatureData) {
						_armatureDataList.erase(_armatureDataList.begin() +i);
					}
				}
			}
		}
	}

	void SkeletonData::removeArmatureDataByName(std::string armatureName) {
		if(name != "") {
			for(int i=0;i<_armatureDataList.size();i++) {
				ArmatureData* data = _armatureDataList[i];
				if(data) {
					if(data->name == armatureName) {
						_armatureDataList.erase(_armatureDataList.begin() + i);
					}
				}
			}
		}
	}

	Vector2D* SkeletonData::getSubTexturePivot(std::string subTextureName) {
		 std::map<std::string,Vector2D*>::iterator it;
		 it = _subTexturePivots.find(subTextureName);
		 if(it != _subTexturePivots.end()) {
			 Vector2D* pivot = it->second;
			 return pivot;
		 } else {
			 printf("Could not find subTexturePivot! \n");
		 }
		 return new Vector2D(0,0);
	}

	Vector2D* SkeletonData::addSubTexturePivot(float x, float y, std::string name) {
		Vector2D* pivot = new Vector2D(x,y);
		_subTexturePivots.insert(std::make_pair(name, pivot));
		return pivot;
	}

	void SkeletonData::removeSubTexturePivot(std::string subTextureName) {
		_subTexturePivots.erase(subTextureName);
	}

};