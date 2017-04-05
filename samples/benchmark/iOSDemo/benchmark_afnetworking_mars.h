// Tencent is pleased to support the open source community by making Mars available.
// Copyright (C) 2016 THL A29 Limited, a Tencent company. All rights reserved.

// Licensed under the MIT License (the "License"); you may not use this file except in
// compliance with the License. You may obtain a copy of the License at
// http://opensource.org/licenses/MIT

// Unless required by applicable law or agreed to in writing, software distributed under the License is
// distributed on an "AS IS" basis, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
// either express or implied. See the License for the specific language governing permissions and
// limitations under the License.
//
//  benchmark_afnetworking_mars.h
//  iOSDemo
//
//  Created by zhouzhijie on 17/1/10.
//  Copyright © 2017年 zhouzhijie. All rights reserved.
//

#ifndef benchmark_afnetworking_mars_h
#define benchmark_afnetworking_mars_h
#import "UINotifyDelegate.h"

@interface BenchMark : NSObject<UINotifyDelegate> {
}
@property(nonatomic, assign) int runtimes;
@property(nonatomic, assign) int bytes;
@property(nonatomic, assign, readonly) BOOL isRunning;
@property(nonatomic, assign) BOOL afRunning;
@property(nonatomic, assign) BOOL marsRunning;
@property(nonatomic, copy) void (^RunStateCallback)();
@property(nonatomic, strong) NSMutableString* logString;

+ (instancetype)shared;
-(void) StartAfnetworkingTest;
-(void) StartMarsTest;
-(void) StartSensitivityTest;

@end



#endif /* benchmark_afnetworking_mars_h */
