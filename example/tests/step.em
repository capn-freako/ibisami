@# Example input file for `run_tests.py'.
@# 
@# This tests the step response of the model and channel.
@#
@# Original Author: David Banas
@# Original Date:   July 20, 2012
@# 
@# Copyright (c) 2012 David Banas; All rights reserved World wide.

<test>
    <name>@(name)</name>
    <result>Visual</result>
    <description>Step response for: @(description)</description>
    <output>
@{
from pylab import *
from numpy import cumsum, concatenate
import pyibisami.amimodel as ami
cla()
for cfg in data:
    cfg_name = cfg[0]
    params = cfg[1]
    if(len(cfg) > 2):
        reference = ref_dir + '/' + name.split()[0] + '/' + cfg[2]
    else:
        reference = None
    initializer = ami.AMIModelInitializer(params[0])
    items = params[1].items()
    items.sort(reverse=True)
    for item in items:
        exec ('initializer.' + item[0] + ' = ' + repr(item[1]))
    model.initialize(initializer)
    print '        <block name="Model Initialization (' + cfg_name + ')" type="text">'
    print "MUT:"
    print model.msg
    print model.ami_params_out
    h = model.initOut
    T = model.sample_interval
    s = cumsum(h) * T
    # In the GetWave case, make 4 calls, to test for proper "stitching".
    s2 = model.getWave(array([0.0, 1.0] + [1.0] * (len(h) - 2)), row_size=len(h))
    t = array([i * T for i in range(len(h))])
    rgb_main, rgb_ref = plot_colors.next()
    color_main = "#%02X%02X%02X" % (rgb_main[0] * 0xFF, rgb_main[1] * 0xFF, rgb_main[2] * 0xFF)
    color_ref = "#%02X%02X%02X" % (rgb_ref[0] * 0xFF, rgb_ref[1] * 0xFF, rgb_ref[2] * 0xFF)
    plot(t * 1.e9, s, label=cfg_name+'_Init', color=color_main)
    plot(t * 1.e9, s2, '.', label=cfg_name+'_GetWave', color=color_main)
    if(reference):
        try:
            ref = ami.AMIModel(reference)
            initializer.root_name = 'easic_rx'
            ref.initialize(initializer)
            print "Reference:"
            print ref.msg
            print ref.ami_params_out
            href = ref.initOut
            r = cumsum(href) * T
        except:
            r = ami.interpFile(reference, T)
        plot(t * 1.e9, r, label=cfg_name+'_ref', color=color_ref)
    print '        </block>'
title('Step Response (V)')
xlabel('Time (nsec.)')
axis(xmax=10e9*model.bit_time)
legend()
filename = plot_names.next()
savefig(filename)
}
        <block name="Step Response" type="image">@(filename)</block>
    </output>
</test>

