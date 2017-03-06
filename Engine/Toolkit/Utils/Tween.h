/*
 * Tween.h
 *
 *  Created on: 2016-6-8
 *      Author: scott.cgi
 */

#ifndef tween_h
#define tween_h

#include <stddef.h>
#include <stdbool.h>

#include "Engine/Toolkit/Math/TweenEase.h"
#include "Engine/Toolkit/Utils/ArrayList.h"

typedef float (*TweenActionValueOnGet)(void* target);
typedef void  (*TweenActionValueOnSet)(void* target, float value);

typedef struct
{
	void*                 userData;

	float                 value;
	float                 fromValue;
	float                 toValue;

	/**
	 * When TweenAction's target value need to get
	 */
	TweenActionValueOnGet OnGet;

	/**
	 * When TweenAction's target value need to set
	 */
	TweenActionValueOnSet OnSet;

	/**
	 * The motion is relative or absolute default true
	 */
	bool                  isRelative;

	/**
	 * Default tween_ease_linear
	 */
	TweenEaseType         easeType;
}
TweenActionValue;


typedef struct TweenAction TweenAction;
typedef void   (*TweenActionOnComplete)(TweenAction* action, void* userData);

struct TweenAction
{
	/**
	 * Bind data can not get from context
	 */
	void*                        userData;

	/**
	 * Target execute TweenAction
	 */
	void*                        target;

	float                        duration;
	float                        curTime;

	/**
	 * Means action running in queue or immediately default true
	 */
	bool                         isQueue;

	/**
	 * TweenActionValue list Transform concurrently
	 */
	ArrayList(TweenActionValue)  actionValueList[1];

	/**
	 * When action complete call back, passed action and userData
	 */
	TweenActionOnComplete        OnComplete;
};


struct ATween
{
	/**
	 * Control by ATween when action complete
	 *
	 * userData   is NULL
	 * target     is NULL
	 * isQueue    is true
	 * duration   is 0
	 * OnComplete is NULL
	 */
	TweenAction*      (*GetAction)            ();

	/**
	 * Add TweenActon's TweenActionValue
	 */
	TweenActionValue* (*AddTweenActionValue)  (TweenAction* action);

	/**
	 * Bind TweenActions to tweenId and running
	 * if tweenId is NULL will generate tweenId value
	 *
	 * return tweenId
	 */
	void*             (*RunActions)           (Array(TweenAction*)* actions, void* tweenId);

	/**
	 * Remove tweenId's all actions immediately, return false when tweenId not in use
	 * we can or not clean up tweenId bound data for actions
	 * when tweenId not in use must cleanup for reuse memory
	 */
    bool              (*TryRemoveAllActions)  (void* tweenId);

	/**
	 * Complete tweenId's all actions immediately, return false when tweenId not in use
	 * if isFireOnComplete true will fire callback
	 */
	bool              (*TryCompleteAllActions)(void* tweenId, bool isFireOnComplete);

    /**
     * Find TweenAction in current or queue, and remove it
     * if tweenId not in use return false
     * if not found TweenAction return false
     */
    bool              (*TryRemoveAction)      (void* tweenId, TweenAction* action);

    /**
     * Whether the tweenId has action in current or queue
     */
	bool              (*HasAction)            (void* tweenId);

    /**
     * Called every frame
     */
	void              (*Update)               (float deltaTime);
};

extern struct ATween ATween[1];

#endif