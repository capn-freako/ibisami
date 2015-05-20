/*! \file amimodel.cpp
*   \brief Implementation of AMIModel class.
*
* Original author: David Banas<br>
* Original date:   May 2, 2015
*
* Copyright (c) 2015 David Banas; all rights reserved World wide.
*/

#include <string>
#include <utility>
#include <vector>
#include "include/amimodel.h"

//! Initialize the model.
void AMIModel::init(double *impulse_matrix, const long number_of_rows,
    const long aggressors, const double sample_interval,
    const double bit_time, const std::string& AMI_parameters_in) {
    impulse_matrix_ = impulse_matrix;
    number_of_rows_ = number_of_rows;
    aggressors_ = aggressors;
    sample_interval_ = sample_interval;
    bit_time_ = bit_time;
    ParseRes res = parse_params(AMI_parameters_in);
    if (!res.first)
        throw std::runtime_error(res.second);
}

//! Parse the incoming AMI parameter string.
ParseRes AMIModel::parse_params(const std::string& AMI_parameters_in) {
    using boost::spirit::ascii::space;
    using boost::spirit::qi::phrase_parse;

    ibisami::AMIGrammar<std::string::const_iterator> the_grammar;
    std::string::const_iterator iter(AMI_parameters_in.begin());
    std::string::const_iterator end(AMI_parameters_in.end());

    bool res = phrase_parse(iter, end, the_grammar, space, param_tree_);
    if (!res || iter != end)
        return ParseRes(false, std::string(iter, end));

    name_ = param_tree_.name;
    return ParseRes(true, std::string(""));
}

/// Returns the integer value of a particular Integer parameter.
/**
* Inputs:
*  - node_names: A vector of strings containing the AMI parameter tree node
*                names required to traverse our way to the parameter of
*                interest. The root name should not be included.
*
*  - default_val: The value to return, if the parameter is not found in the tree.
*
* Returns:
*  - the requested parameter's integer value, if the parameter was found and
*    an integer was able to be scanned from its value string.
*  - 'default_val', if the parameter was not found in the tree.
*
* Throws:
*  - std::runtime_error, if the parameter was found and an integer could not
*    be scanned from its value string.
*/
long AMIModel::get_param_int(const std::vector<std::string>& node_names,
                             long default_val) const {
    std::string param_val_str = get_param(node_names);
    if (param_val_str == "")
        return default_val;
    long res;
    int scans = sscanf(param_val_str.c_str(), "%ld", &res);
    if (scans == 1) {
        return res;
    } else {
        std::ostringstream err;
        err << "AMIModel::get_param_int() could not scan an integer from '"
        << param_val_str << "', while fetching parameter: ";
        std::string err_str = err.str();
        for (std::string node_name : node_names)
            err_str += node_name + ".";
        err_str += "\n";
        throw std::runtime_error(err_str);
    }
}

/// Returns the string value of a particular AMI parameter.
/**
* Inputs:
*  - node_names: A vector of strings containing the AMI parameter tree node
*                names required to traverse our way to the parameter of
*                interest. The root name should not be included.
*
* Returns:
*  - the requested parameter's value string, if the parameter was found.
*  - empty string, otherwise.
*/
std::string AMIModel::get_param(const std::vector<std::string>& node_names) const {
    return get_leaf(param_tree_, &node_names[0], node_names.size());
}

/// Recursively searches the parameter tree, returning the string value of a particular parameter, if found.
/**
* Inputs:
*  - param_tree: The instance of ibisami::ParamTree to search.
*
*  - node_names: A vector of strings containing the AMI parameter tree node
*                names required to traverse our way to the parameter of
*                interest.
*
* Returns:
*  - The requested parameter's value string, if the parameter was found.
*  - An empty string, otherwise.
*/
std::string AMIModel::get_leaf(const ibisami::ParamTree& param_tree,
                               const std::string* name_ptr,
                               size_t num_names) const {
    const ibisami::ParamTree *next_subtree;
    for (const ibisami::ParamNode& node : param_tree.children) {
        next_subtree = boost::apply_visitor(ibisami::Node(), node);
        if (next_subtree) {
            if (next_subtree->name == *name_ptr) {
                if (num_names == 1)
                    return boost::apply_visitor(ibisami::Value(), next_subtree->children[0]);
                else
                    return get_leaf(*next_subtree, ++name_ptr, num_names - 1);
            }
        }
    }
    return "";
}

