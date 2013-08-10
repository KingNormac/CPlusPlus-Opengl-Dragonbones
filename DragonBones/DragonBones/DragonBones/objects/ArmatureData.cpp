#include "ArmatureData.h"
#include <map>
#include <algorithm> 

namespace DragonBones {
	std::vector<BoneData*> ArmatureData::getBoneDataList() {
		return this->_boneDataList;
	}

	std::vector<SkinData*> ArmatureData::getSkinDataList() {
		return this->_skinDataList;
	}

	std::vector<AnimationData*> ArmatureData::getAnimationDataList() {
		return this->_animationDataList;
	}

	ArmatureData::ArmatureData() {
	}

	ArmatureData::~ArmatureData() {
		for(int b=0; b<_boneDataList.size();b++) {
			BoneData* boneData = _boneDataList[b];
			if(boneData) {
				delete[] boneData;
			}
		}

		for(int s=0;s<_skinDataList.size();s++) {
			SkinData* skinData = _skinDataList[s];
			if(skinData) {
				delete[] skinData;
			}
		}

		for(int a=0;a<_animationDataList.size();a++) {
			AnimationData* animationData = _animationDataList[a];
			if(animationData) {
				delete[] animationData;
			}
		}
		_boneDataList.clear();
		_skinDataList.clear();
		_animationDataList.clear();
	}

	BoneData* ArmatureData::getBoneData(std::string boneName) {
		for(int i=0; i<_boneDataList.size();i++) {
			BoneData* boneData = _boneDataList[i];
			if(boneData) {
				if(boneData->name == boneName) {
					return boneData;
				}
			}
		}
		return 0;
	}

	SkinData* ArmatureData::getSkinData(std::string skinName) {
		for(int i=0; i<_skinDataList.size();i++) {
			SkinData* skinData = _skinDataList[i];
			if(skinData) {
				if(skinData->name == skinName) {
					return skinData;
				}
			}
		}
		return 0;
	}

	AnimationData* ArmatureData::getAnimationData(std::string animationName) {
		for(int i=0; i<_animationDataList.size();i++) {
			AnimationData* animationData = _animationDataList[i];
			if(animationData) {
				if(animationData->name == animationName) {
					return animationData;
				}
			}
		}
		return 0;
	}

	void ArmatureData::addBoneData(BoneData* boneData) {
		if(boneData) {
			_boneDataList.push_back(boneData);
		} else {
			printf("Tried to add bad BoneData to ArmatureData \n");
		}
	}

	void ArmatureData::addSkinData(SkinData* skinData) {
		if(skinData) {
			_skinDataList.push_back(skinData);
		} else {
			printf("Tried to add bad SkinData to ArmatureData \n");
		}
	}

	void ArmatureData::addAnimationData(AnimationData* animationData) {
		if(animationData) {
			_animationDataList.push_back(animationData);
		} else {
			printf("Tried to add bad AnimationData to ArmatureData \n");
		}
	}

	void ArmatureData::sortBoneDataList() {
		std::vector<std::pair<int, BoneData*>> sortMap; // maps will auto sort by the ints
		if(_boneDataList.size() == 0) {
			return;
		}
		
		for(int i=0; i<_boneDataList.size();i++) {
			BoneData* boneData = _boneDataList[i];
			int level = 0;
			BoneData* parentData = boneData;
			while(parentData && parentData->parent != "") {
				level++;
				parentData = getBoneData(parentData->parent);
			}
			sortMap.push_back(std::make_pair(level, boneData));
		}

		std::sort(sortMap.begin(), sortMap.end());

		for(int s=0; s<sortMap.size(); s++) {
			//printf("Name: %s with level:%i \n", sortMap[s].second->name.c_str(), sortMap[s].first);
			_boneDataList[s] = sortMap[s].second;
		}		
	}
};