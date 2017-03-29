//
//  BenchmarkViewController.m
//  iOSDemo
//
//  Created by hzy on 27/03/2017.
//  Copyright © 2017 曹少琨. All rights reserved.
//

#import "BenchmarkViewController.h"
#import "benchmark_afnetworking_mars.h"
@interface BenchmarkViewController ()
{
    BenchMark* benchmark;
}
@end

@implementation BenchmarkViewController
- (void)viewDidLoad {
    [super viewDidLoad];
    benchmark = [[BenchMark alloc] init];
//
    
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
}
- (IBAction)onRunAF:(id)sender {
    [benchmark StartAfnetworkingTest];
}
- (IBAction)onRunMar:(id)sender {
    [benchmark StartMarsTest];
}
- (IBAction)onRunSensitivity:(id)sender {
    [benchmark StartSensitivityTest];
}
/*
#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
}
*/

@end
