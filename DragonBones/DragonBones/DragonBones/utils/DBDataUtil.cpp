#include "DBDataUtil.h"
#include "TransformUtil.h"
#include "../objects/TransformFrame.h"
#include "../animation/TimelineState.h"


namespace DragonBones {
	namespace DBDataUtil {

		void transformArmatureData(ArmatureData* armatureData) {
			BoneData* boneData;
			BoneData* parentBoneData;
			for(int i = 0; i != armatureData->_boneDataList.size(); i++) {
				boneData = armatureData->_boneDataList[i];
				if(boneData->parent != "") {
					parentBoneData = armatureData->getBoneData(boneData->parent);
					if(parentBoneData) {
						boneData->transform->copy(boneData->global);
						//printf("Name:%s \n", boneData->name.c_str());
						//printf(" X:%f Y:%f \n", boneData->transform->x, boneData->transform->y);
						TranformUtils::transformPointWithParent(boneData->transform, parentBoneData->global);
						//printf(" X:%f Y:%f \n", boneData->transform->skewX, boneData->transform->skewY);
					}
				}
			}
		}

		void transformArmatureDataAnimations(ArmatureData* armatureData) {
			for(int i =0; i< armatureData->_animationDataList.size(); i++) {
				AnimationData* animationData = armatureData->_animationDataList[i];
				if(animationData) {
					transformAnimationData(animationData, armatureData);
					
				}
			}
		}

		void transformAnimationData(AnimationData* animationData, ArmatureData* armatureData) {
			SkinData* skinData = armatureData->getSkinData("");
			int i = armatureData->_boneDataList.size();
			BoneData* boneData=0;
			TransformTimeline* timeline=0;
			SlotData* slotData=0;
			DisplayData* displayData=0;
			TransformTimeline* parentTimeline=0;
			std::vector<Frame*> frameList;
			DBTransform* originTransform=0;
			Vector2D* originPivot=0;
			TransformFrame* prevFrame=0;
			int frameListLength=0;
			TransformFrame* frame=0;
			DBTransform* _helpTransform1 = new DBTransform();
			DBTransform* _helpTransform2 = new DBTransform();

			while(i > 0) {
				i--;
				boneData = armatureData->_boneDataList[i];
				timeline = animationData->getTimeLine(boneData->name);
				if(!timeline) {
					continue;
				}

				//printf("%s \n", boneData->name.c_str());
				//slotData = skinData->getSlotData(boneData->name);
				for(SlotData* da: skinData->_slotDataList) {
					if(da) {
						if(da->parent == boneData->name) {
							slotData = da;
							break;
						}
					}
				}


				if(boneData->parent != "") {
					parentTimeline = animationData->getTimeLine(boneData->parent);
				} else {
					parentTimeline = 0;
				}

				frameList = timeline->_frameList;

				originTransform = 0;
				originPivot = 0;
				prevFrame = 0;
				frameListLength = frameList.size();
				for(int j=0;j<frameListLength;j++) {
					frame = (TransformFrame*)frameList[j];
					if(parentTimeline) {
						_helpTransform1->copy(frame->global);

						getTimelineTransform(parentTimeline, frame->position, _helpTransform2);
						TranformUtils::transformPointWithParent(_helpTransform1, _helpTransform2);

						frame->transform->copy(_helpTransform1);
					} else {
						frame->transform->copy(frame->global);
					}

					frame->transform->x -= boneData->transform->x;
					frame->transform->y -= boneData->transform->y;
					frame->transform->skewX -= boneData->transform->skewX;
					frame->transform->skewY -= boneData->transform->skewY;
					frame->transform->scaleX -= boneData->transform->scaleX;
					frame->transform->scaleY -= boneData->transform->scaleY;


					if(!timeline->transformed)
					{
						if(slotData)
						{
							frame->zOrder -= slotData->zOrder;
						}
					}


					if(!originTransform) {
						originTransform = timeline->originTransform;
						originTransform->copy(frame->transform);
						originTransform->skewX = TranformUtils::formatRadian(originTransform->skewX);
						originTransform->skewY = TranformUtils::formatRadian(originTransform->skewY);
						originPivot = timeline->originPivot;
						originPivot->x = frame->pivot->x;
						originPivot->y = frame->pivot->y;
					}


					frame->transform->x -= originTransform->x;
					frame->transform->y -= originTransform->y;
					float sx = frame->transform->skewX - originTransform->skewX;
					float sy = frame->transform->skewY - originTransform->skewY;
					frame->transform->skewX = TranformUtils::formatRadian(sx);
					frame->transform->skewY = TranformUtils::formatRadian(sy);
					frame->transform->scaleX -= originTransform->scaleX;
					frame->transform->scaleY -= originTransform->scaleY;
					

					if(!timeline->transformed)
					{
						frame->pivot->x -= originPivot->x;
						frame->pivot->y -= originPivot->y;
					}

					double PI = atan(1.0)*4;
					if(prevFrame) {
						float dLX = frame->transform->skewX - prevFrame->transform->skewX;
						if(prevFrame->tweenRotate) {
							if(prevFrame->tweenRotate > 0) {
								if(dLX < 0) {
									frame->transform->skewX += PI * 2;
									frame->transform->skewY += PI * 2;
								}

								if(prevFrame->tweenRotate > 1) {
									frame->transform->skewX += PI * 2 * (prevFrame->tweenRotate - 1);
									frame->transform->skewY += PI * 2 * (prevFrame->tweenRotate - 1);
								}
							} else {
								if(dLX > 0) {
									frame->transform->skewX -= PI * 2;
									frame->transform->skewY -= PI * 2;
								}

								if(prevFrame->tweenRotate < 1) {
									frame->transform->skewX += PI * 2 * (prevFrame->tweenRotate + 1);
									frame->transform->skewY += PI * 2 * (prevFrame->tweenRotate + 1);
									//printf("%s \n", frame->transform->toString().c_str());
									//printf("%i \n", j);
								}
							}
							
						} else {
							frame->transform->skewX = prevFrame->transform->skewX + TranformUtils::formatRadian(frame->transform->skewX - prevFrame->transform->skewX);
							frame->transform->skewY = prevFrame->transform->skewY + TranformUtils::formatRadian(frame->transform->skewY - prevFrame->transform->skewY);
						}

					}

					//printf("%s \n \n", frame->transform->toString().c_str());

					prevFrame = frame;
				}
				timeline->transformed = true;
			}
		}

		void getTimelineTransform(TransformTimeline* timeline, float position, DBTransform* result) {
			std::vector<Frame*> frameList = timeline->_frameList;
			int i = frameList.size();

			TransformFrame* currentFrame;
			float tweenEasing;
			float progress;
			TransformFrame* nextFrame;
			while(i > 0) {
				i--;
				currentFrame = (TransformFrame*)frameList[i];
				if(currentFrame->position <= position && (currentFrame->position + currentFrame->duration > position)) {
					tweenEasing = currentFrame->tweenEasing;
					if(i == frameList.size() - 1 || tweenEasing != tweenEasing || position == currentFrame->position) {
						result->copy(currentFrame->global);
					} else {
						progress = (position - currentFrame->position) / currentFrame->duration;
						if(tweenEasing)
						{
							progress = TimelineState::getEaseValue(progress, tweenEasing);
						}

						nextFrame = (TransformFrame*) timeline->_frameList[i + 1];

						result->x = currentFrame->global->x + (nextFrame->global->x - currentFrame->global->x) * progress;
						result->y = currentFrame->global->y + (nextFrame->global->y - currentFrame->global->y) * progress;
						result->skewX = TranformUtils::formatRadian(currentFrame->global->skewX +  (nextFrame->global->skewX - currentFrame->global->skewX) * progress);
						result->skewY = TranformUtils::formatRadian(currentFrame->global->skewY +  (nextFrame->global->skewY - currentFrame->global->skewY) * progress);
						result->scaleX = currentFrame->global->scaleX +  (nextFrame->global->scaleX - currentFrame->global->scaleX) * progress;
						result->scaleY = currentFrame->global->scaleY +  (nextFrame->global->scaleY - currentFrame->global->scaleY) * progress;
					}
					break;
				}
			}
		}

		void addHideTimeline(AnimationData* animationData, ArmatureData* armatureData) {
			std::vector<BoneData*> boneDataList = armatureData->_boneDataList;
			int i = boneDataList.size();

			BoneData* boneData;
			std::string boneName;
			while(i > 0) {
				i--;
				boneData = boneDataList[i];
				boneName = boneData->name;
				if(!animationData->getTimeLine(boneName)) {
					animationData->addTimeLine(TransformTimeline::HIDE_TIMELINE, boneName);
				}
			}
		}
	};
};