// Tencent is pleased to support the open source community by making Mars available.
// Copyright (C) 2016 THL A29 Limited, a Tencent company. All rights reserved.

// Licensed under the MIT License (the "License"); you may not use this file except in 
// compliance with the License. You may obtain a copy of the License at
// http://opensource.org/licenses/MIT

// Unless required by applicable law or agreed to in writing, software distributed under the License is
// distributed on an "AS IS" basis, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
// either express or implied. See the License for the specific language governing permissions and
// limitations under the License.

/*
 * active_logic.cc
 *
 *  Created on: 2012-8-22
 *      Author: yerungui
 */

/**
 baseevent-mac项目，定义了几种常用信号，并提供了函数调用和函数指针两种获取方式。
 
 active_logic:
 静态方法，启动加载，监听onforeground信号，使ActiveLogic单例响应处理。
 若处理线程并不为MessageQueue::GetDefMessageQueue()则push进栈。
 若前后台状态有变化，触发SignalActive信号方法。
 */

#include "boost/bind.hpp"

#include "mars/baseevent/active_logic.h"
#include "mars/baseevent/baseprjevent.h"
#include "mars/comm/singleton.h"
#include "mars/comm/xlogger/xlogger.h"
#include "mars/comm/thread/lock.h"
#include "mars/comm/time_utils.h"
#include "mars/comm/bootrun.h"
#include "mars/comm/messagequeue/message_queue.h"

static void onForeground(bool _isforeground) {
    SINGLETON_STRONG(ActiveLogic)->OnForeground(_isforeground);//hzy: 3.3
}

static void __initbind_baseprjevent() {
    GetSignalOnForeground().connect(&onForeground);//hzy: 3.2
}

BOOT_RUN_STARTUP(__initbind_baseprjevent);//hzy: 3.1 boost::signal信号绑定Active相关消息

#ifdef ANDROID
#define INACTIVE_TIMEOUT (10*60*1000) //ms
#elif defined Q_OS_BLACKBERRY
#define INACTIVE_TIMEOUT (10*60*1000) //ms
#elif defined _WIN32
#define INACTIVE_TIMEOUT (10*60*1000) //ms
#else
#define INACTIVE_TIMEOUT (10*60*1000) //ms
#endif


ActiveLogic::ActiveLogic()
: isforeground_(false), isactive_(true)
, alarm_(boost::bind(&ActiveLogic::__OnInActive, this), false)
, lastforegroundchangetime_(::gettickcount())
{
    xinfo_function();
#ifndef __APPLE__
        if (!alarm_.Start(INACTIVE_TIMEOUT))
       	{
            xerror2(TSF"m_alarm.Start false");
    	}
#endif
}

ActiveLogic::~ActiveLogic()
{
    xinfo_function();
	MessageQueue::CancelMessage(MessageQueue::DefAsyncInvokeHandler(MessageQueue::GetDefMessageQueue()), (MessageQueue::MessageTitle_t)this);
	MessageQueue::WaitForRuningLockEnd(MessageQueue::DefAsyncInvokeHandler(MessageQueue::GetDefMessageQueue()));
}

void ActiveLogic::OnForeground(bool _isforeground)
{
	if (MessageQueue::GetDefMessageQueue()!=MessageQueue::CurrentThreadMessageQueue())
	{//hzy: 3.4 add signal to message queue.
        MessageQueue::AsyncInvoke(boost::bind(&ActiveLogic::OnForeground, this, _isforeground), (MessageQueue::MessageTitle_t)this, mq::DefAsyncInvokeHandler(mq::GetDefMessageQueue()));
		return;
	}

    xgroup2_define(group);
    xinfo2(TSF"OnForeground:%0, change:%1, ", _isforeground, _isforeground!=isforeground_) >> group;

    if (_isforeground == isforeground_) return;

    bool oldisactive = isactive_;
    isactive_ = true;
    isforeground_ = _isforeground;
    lastforegroundchangetime_ = ::gettickcount();
    alarm_.Cancel();

    if (!isforeground_)
    {
#ifndef __APPLE__
        if (!alarm_.Start(INACTIVE_TIMEOUT))
       	{
            xerror2(TSF"m_alarm.Start false") >> group;
    	}
#endif
    }

    bool isnotify = oldisactive!=isactive_;
    SignalForeground(isforeground_);

    if (isnotify)
    {
    	xinfo2(TSF"active change:%0", isactive_) >> group;
    	SignalActive(isactive_);
    }
}

bool ActiveLogic::IsActive() const
{
    return isactive_;
}

bool ActiveLogic::IsForeground() const
{
	return isforeground_;
}

uint64_t ActiveLogic::LastForegroundChangeTime() const
{
	return lastforegroundchangetime_;
}

void ActiveLogic::__OnInActive()
{
    xdebug_function();
    if (!isforeground_) isactive_ = false;

    bool  isactive = isactive_;
    xinfo2(TSF"active change:%0", isactive_);
    SignalActive(isactive);
}
