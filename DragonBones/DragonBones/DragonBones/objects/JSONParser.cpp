#include "JSONParser.h"
#include <fstream>
#include "../utils/DBDataUtil.h"
#include "../../json/json.h"

namespace DragonBones {
	 TextureAtlasData* JSONParser::parseTextureAtlas(std::string filename, float scale) {
		Json::Value root;
		Json::Reader reader;
		std::ifstream test(filename, std::ifstream::binary);
		if(!reader.parse(test, root, false)) {
			printf("JSONParser failed to read textureAtlas: %s \n", filename.c_str());
			return 0;
		}
		TextureAtlasData* data = new TextureAtlasData(root["name"].asString());
		for(Json::Value subTextureValue : root["SubTexture"]) {
			SubTextureData* subData = new SubTextureData(
				subTextureValue["name"].asString(),
				subTextureValue["x"].asInt(),
				subTextureValue["y"].asInt(),
				subTextureValue["width"].asInt(),
				subTextureValue["height"].asInt());
			data->addSubTextureData(subData);
			//printf("Name:%s, x:%i, y:%i, w:%i, h:%i \n", subData->name.c_str(), subData->x, subData->y, subData->width, subData->height);
		}
		return data;
	}

	SkeletonData* JSONParser::parseSkeletonData(std::string filename) {
		Json::Value root;
		Json::Reader reader;
		std::ifstream test(filename, std::ifstream::binary);
		if(!reader.parse(test, root, false)) {
			printf("JSONParser failed to read skeletonData: %s \n", filename.c_str());
			return 0;
		}
		if(root["version"].asFloat() != 2.3f) {
			printf("JSONParser: bad skeletondata version: %s \n", filename.c_str());
			return 0;
		}
		SkeletonData* data=new SkeletonData();
		data->name = root["name"].asString();
		int framerate = root["frameRate"].asInt();
		for(Json::Value armatureValue : root["armature"]) {
			data->addArmatureData(parseArmatureData(armatureValue, data, framerate));
		}
		return data;
	}

	ArmatureData* JSONParser::parseArmatureData(Json::Value armatureValue, SkeletonData* data, int framerate) {
		ArmatureData* armatureData = new ArmatureData();
		armatureData->name = armatureValue["name"].asString();
		//printf("ArmatureName: %s \n", armatureData->name.c_str());

		for(Json::Value boneValue : armatureValue["bone"]) {
			armatureData->addBoneData(parseBoneData(boneValue));
		}

		for(Json::Value skinValue : armatureValue["skin"]) {
			SkinData* skinData = parseSkinData(skinValue, data);
			armatureData->addSkinData(skinData);
		}

		DBDataUtil::transformArmatureData(armatureData);
		armatureData->sortBoneDataList();

		for(Json::Value animationValue : armatureValue["animation"]) {
			armatureData->addAnimationData(parseAnimationData(animationValue, armatureData, framerate));
		}
		return armatureData;
	}

	 BoneData* JSONParser::parseBoneData(Json::Value boneValue) {
		BoneData* boneData = new BoneData();
		boneData->name = boneValue["name"].asString();
		boneData->parent = boneValue["parent"].asString();
		boneData->length = boneValue.get("Length",0).asFloat();
		//printf("BoneName:%s Parent:%s Length:%f \n", boneData->name.c_str(), boneData->parent.c_str(), boneData->length);
			
		parseTransform(boneValue["transform"], boneData->global);
		boneData->transform->copy(boneData->global);
		//if(boneData->parent != "")
		//	printf("%s with parent %s \n", boneData->name.c_str(), boneData->parent.c_str());
		return boneData;
	}

	 SkinData* JSONParser::parseSkinData(Json::Value skinValue, SkeletonData* data) {
		 SkinData* skinData = new SkinData();
		 skinData->name = skinValue["name"].asString();
		
		 for(Json::Value slotValue : skinValue["slot"]) {
			 skinData->addSlotData(parseSlotData(slotValue, data));
		 }

		 return skinData;
	}

	SlotData* JSONParser::parseSlotData(Json::Value slotValue, SkeletonData* data) {
		SlotData* slotData = new SlotData();
		slotData->name = slotValue["name"].asString();
		slotData->parent = slotValue["parent"].asString();
		slotData->zOrder = slotValue["z"].asInt();

		for(Json::Value displayValue : slotValue["display"]) {
			slotData->addDisplayData(parseDisplayData(displayValue, data));
		}

		return slotData;
	}

	DisplayData* JSONParser::parseDisplayData(Json::Value displayValue, SkeletonData* data) {
		DisplayData* displayData = new DisplayData();
		displayData->name = displayValue["name"].asString();
		displayData->type = displayValue["type"].asString();
		 
		displayData->pivot = data->addSubTexturePivot(0,0, displayData->name);
		parseTransform(displayValue["transform"], displayData->transform, displayData->pivot);
			
		return displayData;
	}

	AnimationData* JSONParser::parseAnimationData(Json::Value animationValue, ArmatureData* armatureData, int framerate) {
		AnimationData* animationData = new AnimationData();
		animationData->name = (std::string) animationValue.get("name", "").asString();
		animationData->framerate = framerate;
		//animationData->loop = animationValue["loop"].asInt();
		animationData->loop = animationValue.get("loop", 1).asInt();
		animationData->setFadeInTime(animationValue.get("fadeInTime", -1).asFloat());
		animationData->setDuration(animationValue["duration"].asFloat()/framerate);
		animationData->setScale(animationValue["scale"].asFloat());
		//animationData->tweenEasing = animationValue["tweenEasing"].asFloat();
		animationData->tweenEasing = animationValue.get("tweenEasing", _Nan._Float).asFloat();

		parseTimeline(animationValue, animationData, 1, framerate);

		TransformTimeline* timeline;
		std::string timelineName;

		for(Json::Value timelineValue : animationValue["timeline"]) {
			timeline = parseTransformTimeline(timelineValue, animationData->_duration, framerate);
			timelineName = timelineValue["name"].asString();
			animationData->addTimeLine(timeline,timelineName);
		}
		
		DBDataUtil::addHideTimeline(animationData, armatureData);
		DBDataUtil::transformAnimationData(animationData, armatureData);

		return animationData;
	}

	void JSONParser::parseTimeline(Json::Value timelineValue, Timeline* timeline, int type, int framerate) {
		float position = 0;
		Frame* frame = 0;
		for(Json::Value frameValue : timelineValue["frame"]) {
			if(type==2)
				frame = parseTransformFrame(frameValue, framerate);
			else if(type==1)
				frame = parseMainFrame(frameValue, framerate);
			frame->position = position;
			timeline->addFrame(frame);
			position += frame->duration;
		}

		if(frame)
			frame->duration = timeline->_duration - frame->position;
	}

	TransformTimeline* JSONParser::parseTransformTimeline(Json::Value timelineValue, float duration, int framerate) {
		TransformTimeline* timeline = new TransformTimeline();
		timeline->_duration = duration;
		
		parseTimeline(timelineValue, timeline, 2, framerate);
			
		timeline->setScale(timelineValue["scale"].asFloat());
		//timeline->setOffset(timelineValue["offset"].asFloat());
		timeline->setOffset(timelineValue.get("offset", 0).asFloat());
			
		return timeline;
	}

	void JSONParser::parseFrame(Json::Value frameValue, Frame* frame, int framerate) {
		frame->duration = frameValue.get("duration", 0).asFloat() / framerate;
		frame->action = frameValue["action"].asString();
		frame->event = frameValue["event"].asString();
		frame->sound = frameValue["sound"].asString();
	}

	Frame* JSONParser::parseMainFrame(Json::Value frameValue, int framerate) {
		Frame* frame = new Frame();
		parseFrame(frameValue, frame, framerate);
		return frame;
	}

	TransformFrame* JSONParser::parseTransformFrame(Json::Value frameValue, int framerate) {
		TransformFrame* frame = new TransformFrame();
		parseFrame(frameValue, frame, framerate);
		
		frame->visible = (frameValue["hide"].asInt() != 1);
		frame->tweenEasing = frameValue.get("tweenEasing", _Nan._Float).asFloat();
		frame->tweenRotate = frameValue["tweenRotate"].asFloat();
		frame->displayIndex = frameValue["displayIndex"].asInt();
		frame->zOrder = frameValue["z"].asInt();
			
		parseTransform(frameValue["transform"], frame->global, frame->pivot);
		frame->transform->copy(frame->global);
		
		Json::Value colorTransformValue = frameValue["colorTransform"];
		if(colorTransformValue.size() > 1)
		{
			frame->color = new ColorTransform();
			frame->color->alphaOffset = colorTransformValue["aO"].asFloat();
			frame->color->redOffset = colorTransformValue["rO"].asFloat();
			frame->color->greenOffset = colorTransformValue["gO"].asFloat();
			frame->color->blueOffset = colorTransformValue["bO"].asFloat();
				
			frame->color->alphaMultiplier = colorTransformValue["aM"].asFloat() * 0.01;
			frame->color->redMultiplier = colorTransformValue["rM"].asFloat() * 0.01;
			frame->color->greenMultiplier = colorTransformValue["gM"].asFloat() * 0.01;
			frame->color->blueMultiplier = colorTransformValue["bM"].asFloat() * 0.01;
		}	
		return frame;
	}

	void JSONParser::parseTransform(Json::Value transformValue, DBTransform* transform, Vector2D* pivot) {
		if(transform)
		{
			transform->x = transformValue["x"].asFloat();
			transform->y = transformValue["y"].asFloat();
			transform->skewX = transformValue["skX"].asFloat() * (atan(1.0)*4 / 180);
			transform->skewY = transformValue["skY"].asFloat() * (atan(1.0)*4 / 180);
			transform->scaleX = transformValue.get("scX", 1).asFloat();
			transform->scaleY = transformValue.get("scY", 1).asFloat();
			if(pivot)
			{
				pivot->x = transformValue["pX"].asFloat();
				pivot->y = transformValue["pY"].asFloat();
			}
			//printf("%f \n", transform->scaleX);
			//printf(transform->toString().c_str());
		}
	}
}