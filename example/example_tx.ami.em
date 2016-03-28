(@model_name

    (Description "@description")

@{
for (sec_name, sec_key) in [('Reserved_Parameters', 'reserved'), ('Model_Specific', 'model')]:
    print "    (%s" % sec_name
    for param_name in ami_params[sec_key].keys():
        print "        (%s" % param_name
        param = ami_params[sec_key][param_name]
        for (fld_name, fld_key) in [('Usage', 'usage'), ('Type', 'type'), ('Format', 'format'),
                                    ('Default', 'default'), ('Description', 'description'), 
                                   ]:
            # Trap the special cases.
            if(fld_name == 'Type'):
                print "            (Type", param_types[param['type']]['ami_type'], ")"
            elif(fld_name == 'Default'):
                if(param['format'] == 'Value'):
                    pass
            elif(fld_name == 'Format'):
                if(param['format'] == 'Value'):
                    print "            (Value", param['default'], ")"
                else:
                    print "            (%s" % param['format'], param['default'], param['min'], param['max'], ")"
            # Execute the default action.
            else:
                print "            (%s" % fld_name, param[fld_key], ")" 

        print "        )"
    print "    )"
}
)

