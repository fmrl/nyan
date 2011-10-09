#ifndef NYAN_FAIL_POLICY_HPP_IS_INCLUDED
#define NYAN_FAIL_POLICY_HPP_IS_INCLUDED

// acknowledgement: i figured how to use boost::mpl thanks to the generous
// advice provided in a discussion at <http://groups.google.com/group/boost-list/browse_thread/thread/1d879d7512d7f968?pli=1>.

#include <nyan/fail/fail_base.hpp>

#include <nyan/config.h>
#if NYAN_CAN_HAS_YAML
#     include <yaml-cpp/yaml.h>
#endif // NYAN_CAN_HAS_YAMLCPP

#include <boost/mpl/begin.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/mpl/end.hpp>
#include <boost/mpl/next.hpp>
#include <boost/mpl/reverse.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/type_traits/is_same.hpp>

#include <exception>

namespace nyan
{

template < class It, class End, class To, class Policy >
bool apply_fail_policy3(To to_arg, Policy, boost::mpl::true_)
{
   to_arg();
   return false;
}

template < class It, class End, class To, class Policy >
bool apply_fail_policy3(To to_arg, Policy policy_arg, boost::mpl::false_)
{
   typedef typename boost::mpl::deref< It >::type Exception;
   typedef typename boost::mpl::next< It >::type Next;

   try
   {
      return apply_fail_policy3< Next, End >(to_arg, policy_arg,
            typename boost::is_same< Next, End >::type());
   }
   catch (const Exception &e)
   {
      policy_arg(e);
      return true;
   }
}

template < class Protocol, class To, class Policy >
bool apply_fail_policy2(To to_arg, Policy policy_arg)
{
   typedef typename boost::mpl::begin< Protocol >::type Begin;
   typedef typename boost::mpl::end< Protocol >::type End;
   return apply_fail_policy3< Begin, End >(to_arg, policy_arg,
         typename boost::is_same< Begin, End >::type());
}

template < class To, class Policy >
bool apply_fail_policy(To to_arg, Policy policy_arg)
{
   return apply_fail_policy2<
         typename boost::mpl::reverse< typename Policy::protocol >::type
         >(to_arg, policy_arg);
}

class fail_policy
{
public:

   typedef boost::mpl::vector< nyan::fail, std::exception > protocol;

   void operator()(const std::exception &fail_arg) const;
   void operator()(const nyan::fail &fail_arg) const;

};

}

#endif /* NYAN_FAIL_POLICY_HPP_IS_INCLUDED */
