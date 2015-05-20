/** \file amimodel.h
 *  \brief Interface to AMIModel class.
 *
 * Original author: David Banas <br>
 * Original date:   May 1, 2015
 *
 * Copyright (c) 2015 David Banas; all rights reserved World wide.
 *
 * This abstract class provides the common base for all AMI models.
 */

#ifndef INCLUDE_AMIMODEL_H_
#define INCLUDE_AMIMODEL_H_

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/qi_omit.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_fusion.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>
#include <boost/variant.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <fstream>
#include <string>
#include <vector>
#include <utility>

// Define the recursive structure of an AMI parameter tree and import to boost.
// (This requires boost, as we don't yet have polymorphic recursion in C++.)

//! Used to protect several complex custom types from potential name collisions.
namespace ibisami {
    struct ParamTree;
    typedef boost::variant<boost::recursive_wrapper<ParamTree>,
                        std::string> ParamNode;  //!< The actual node definition.
    //! The parameter tree definition.
    //
    //! A parameter tree has two fields:
    //!   - an *identifier* string, and
    //!   - a *node*, which may be either:
    //!     - a string value (in the case of a leaf), or
    //!     - a parameter tree (in the case of a branch).
    struct ParamTree {
        std::string name;  //!< identifier
        std::vector<ParamNode> children;  //!< node
    };

    // This is really clunky, but, as I read the examples and forum posts, is how
    // we have to traverse polymorphic structures in C++, even when we're using
    // Boost; yuk!
    //
    // IF ANYONE KNOWS A BETTER WAY, PLEASE, CONTACT ME. THANKS!
    //
    static std::string empty_str("");
    //! Used to access a *leaf* node.
    struct Value : boost::static_visitor<const std::string&> {
        const std::string& operator() (const ibisami::ParamTree& param_tree) const {return empty_str;}
        const std::string& operator() (const std::string& val_str) const {return val_str;}
    };
    //! Used to access a *branch* node.
    struct Node : boost::static_visitor<const ibisami::ParamTree*> {
        const ibisami::ParamTree* operator() (const ibisami::ParamTree& param_tree) const {return &param_tree;}
        const ibisami::ParamTree* operator() (const std::string& val_str) const {return nullptr;}
    };

    /*! \struct ibisami::AMIGrammar
    * \brief AMI parameter tree grammatical definition
    *
    * Defines the grammatical structure of an AMI parameter tree, so
    * that a completely generic parser can be implemented.
    */
}  // namespace ibisami

BOOST_FUSION_ADAPT_STRUCT(
    ibisami::ParamTree,
    (std::string, name)
    (std::vector<ibisami::ParamNode>, children)
)

namespace ibisami {
    namespace phoenix = boost::phoenix;
    namespace qi      = boost::spirit::qi;
    namespace ascii   = boost::spirit::ascii;
    namespace spirit  = boost::spirit;

    template <typename Iterator>
    struct AMIGrammar : qi::grammar<Iterator, ParamTree(), ascii::space_type> {
        AMIGrammar() : AMIGrammar::base_type(branch) {
            using qi::lit;
            using qi::lexeme;
            using ascii::char_;
            using ascii::alnum;
            using ascii::string;
            using ascii::space;
            using spirit::omit;
            using phoenix::at_c;
            using phoenix::push_back;

            quoted_string %= lexeme['"' >> +(char_ - '"') >> '"'];
            comment = lexeme['|' >> *(char_ - '\n') >> '\n'];
            value %= quoted_string | lexeme[+(char_ - ')')];
            node %= comment || (branch | value);
            branch %= '('
                >> lexeme[+(alnum | char_('_'))]
                >> *node
                >> ')';
        }
        qi::rule<Iterator, ParamTree(), ascii::space_type> branch;
        qi::rule<Iterator, ParamNode(), ascii::space_type> node;
        qi::rule<Iterator, std::string(), ascii::space_type> value;
        qi::rule<Iterator, void(), ascii::space_type> comment;
        qi::rule<Iterator, std::string(), ascii::space_type> quoted_string;
    };
}  // namespace ibisami

typedef std::pair<bool, std::string> ParseRes;

/// Abstract class providing the base functionality required by all IBIS-AMI models.
class AMIModel {
 public:
    virtual ~AMIModel() {}
    virtual void init(double *impulse_matrix, const long number_of_rows,
        const long aggressors, const double sample_interval,
        const double bit_time, const std::string& AMI_parameters_in);  ///< Initialize the model.
    virtual void proc_imp() = 0;  ///< Process the incoming impulse response.
    std::string& msg() {return msg_;}  ///< Retrieve the model message.
    std::string& param_str() {return param_str_;}  ///< Retrieve the model parameter string.

 protected:
    ParseRes parse_params(const std::string& AMI_parameters_in);  ///< Parse the incoming AMI parameter string.
    std::string get_param(const std::vector<std::string>& node_names) const;  ///< Get the string value of a parameter.
    long get_param_int(const std::vector<std::string>& node_names, long default_val) const;  ///< Get the value of an integer parameter.
    void log(std::string msg) {if(log_ && clog_) {clog_ << msg << "\n";
        flush(clog_);}}
    std::string msg_, param_str_, name_;
    double sample_interval_, bit_time_, *impulse_matrix_;
    long number_of_rows_, aggressors_;
    ibisami::ParamTree param_tree_;
    std::ofstream clog_;
    bool log_;

 private:
    std::string get_leaf(const ibisami::ParamTree& param_tree,
        const std::string* name_ptr, size_t num_names) const;
};

#endif  // INCLUDE_AMIMODEL_H_

