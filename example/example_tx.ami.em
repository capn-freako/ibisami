(@model_name

    (Description "@description")

    (Reserved_Parameters
@{
for param_name in ami_params['reserved'].keys():
    param = ami_params['reserved'][param_name]
    print "        (%s" % param_name
    print "            (Usage Info)"
    print "            (Type", param_types[param['type']]['ami_type'], ")"
    print "            (Default", param['default'], ")"
    print '            (Description "%s")' % (param['description'],)
    print "        )"
    print
}
    )

    (Model_Specific
@{
for param_name in ami_params['model'].keys():
    param = ami_params['model'][param_name]
    print "        (%s" % param_name
    print "            (Usage", param['usage'], ")"
    print "            (Type", param_types[param['type']]['ami_type'], ")"
    print "            (Default", param['default'], ")"
    print '            (Description "%s")' % (param['description'],)
    print "        )"
    print
}
    )

)

