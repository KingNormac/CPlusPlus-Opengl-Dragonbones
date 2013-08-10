#ifndef JSONPARSER_H
#define JSONPARSER_H

//#include <json/json.h>
#include "SkeletonData.h"
#include "TransformFrame.h"
#include "../objects/TextureAtlasData.h"

namespace Json {
class Value;
}

namespace DragonBones {
	class JSONParser {
	private:
		static ArmatureData* parseArmatureData(Json::Value armatureValue, SkeletonData* data, int framerate);
		static BoneData* parseBoneData(Json::Value boneValue);
		static SkinData* parseSkinData(Json::Value skinValue, SkeletonData* data);
		static SlotData* parseSlotData(Json::Value slotValue, SkeletonData* data);
		static DisplayData* parseDisplayData(Json::Value displayValue, SkeletonData* data);
		static AnimationData* parseAnimationData(Json::Value animationValue, ArmatureData* armatureData, int framerate);
		static void parseTimeline(Json::Value timelineValue, Timeline* timeline, int type, int framerate);
		static TransformTimeline* parseTransformTimeline(Json::Value timelineValue, float duration, int framerate);
		static void parseFrame(Json::Value frameValue, Frame* frame, int framerate);
		static Frame* parseMainFrame(Json::Value frameValue, int framerate);
		static TransformFrame* parseTransformFrame(Json::Value frameValue, int framerate);
		static void parseTransform(Json::Value transformValue, DBTransform* transform, Vector2D* pivot=0);
	public:
		static TextureAtlasData* parseTextureAtlas(std::string filename, float scale=1);
		static SkeletonData* parseSkeletonData(std::string filename);
	};
}
#endif