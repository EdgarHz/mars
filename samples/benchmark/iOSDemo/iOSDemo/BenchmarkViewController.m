//
//  BenchmarkViewController.m
//  iOSDemo
//
//  Created by hzy on 27/03/2017.
//  Copyright © 2017 曹少琨. All rights reserved.
//

#import "BenchmarkViewController.h"
#import "benchmark_afnetworking_mars.h"
@interface BenchmarkViewController ()<UITextViewDelegate>
{
    
}
@property(nonatomic, strong) BenchMark* benchmark;
@property(nonatomic, strong) NSTimer* timer;
@end

@implementation BenchmarkViewController
- (void)viewDidLoad {
    [super viewDidLoad];
    
    
    self.benchmark = [BenchMark shared];
    __weak BenchmarkViewController* wself = self;
    self.benchmark.RunStateCallback = ^(){
        __strong BenchmarkViewController* sself = wself;
        if (!sself.benchmark.isRunning) {
            [sself enableBenMarkPara:YES];
        }
    };
    UISegmentedControl* rt = [self.view viewWithTag:50];
    UISegmentedControl* bt = [self.view viewWithTag:51];
    self.benchmark.runtimes = 500;
    self.benchmark.bytes = 0;
}
- (void)runTimer:(BOOL)run {
    if (run) {
        __weak BenchmarkViewController* wself = self;
        self.timer = [NSTimer  scheduledTimerWithTimeInterval:1 repeats:YES block:^(NSTimer * _Nonnull timer) {
            __strong BenchmarkViewController* sself = wself;
            [sself asyncUpdateLogText];
        }];
    } else {
        [self.timer invalidate];
        self.timer = nil;
    }
}

- (void)asyncUpdateLogText {
    __weak BenchmarkViewController* wself = self;
    dispatch_async(dispatch_get_main_queue(), ^{
        __strong BenchmarkViewController* sself = wself;
        [sself updateLogText];
    });
}
- (void)updateLogText {
    UITextView* t = [self.view viewWithTag:55];
    t.text = [self.benchmark.logString copy];
}
- (void)enableBenMarkPara:(BOOL)enable {
    [self runTimer:!enable];
    [self asyncUpdateLogText];
    
    UISegmentedControl* rt = [self.view viewWithTag:50];
    UISegmentedControl* bt = [self.view viewWithTag:51];
    UIButton* af = [self.view viewWithTag:52];
    UIButton* mars = [self.view viewWithTag:53];
    UIButton* sen = [self.view viewWithTag:54];
    
    rt.enabled = enable;
    bt.enabled = enable;
    
    af.enabled = enable;
    mars.enabled = enable;
    sen.enabled = enable;
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
}
- (IBAction)onRunAF:(id)sender {
    [self enableBenMarkPara:NO];
    [self.benchmark StartAfnetworkingTest];
}
- (IBAction)onRunMar:(id)sender {
    [self enableBenMarkPara:NO];
    [self.benchmark StartMarsTest];
}
- (IBAction)onRunSensitivity:(id)sender {
    [self enableBenMarkPara:NO];
    [self.benchmark StartSensitivityTest];
}

- (IBAction)onBytesChanged:(UISegmentedControl*)sender {
    switch (sender.selectedSegmentIndex) {
        case 0:
            self.benchmark.bytes = 0;
            break;
        case 1:
            self.benchmark.bytes = 64;
            break;
        case 2:
            self.benchmark.bytes = 128;
            break;
        default:
            break;
    }
    
}
- (IBAction)onRunTimesChanged:(UISegmentedControl*)sender {
    switch (sender.selectedSegmentIndex) {
        case 0:
            self.benchmark.runtimes = 50;
            break;
        case 1:
            self.benchmark.runtimes = 500;
            break;
        default:
            break;
    }
}
- (BOOL)textViewShouldEndEditing:(UITextView *)textView {
    return YES;
}
-(BOOL)textView:(UITextView *)textView shouldChangeTextInRange:(NSRange)range replacementText:(NSString *)text {
    if ([text isEqualToString:@"\n"])
    {
        [textView resignFirstResponder];
        return NO;
    }
    return NO;
}
@end
