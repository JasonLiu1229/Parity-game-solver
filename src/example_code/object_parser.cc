#include <iostream>
#include <spot/parseaut/public.hh>
#include <spot/twaalgos/postproc.hh>
#include <spot/twaalgos/hoa.hh>

int main()
{
  spot::parsed_aut_ptr pa = parse_aut("../HOA_files/tut30.hoa", spot::make_bdd_dict());
  if (pa->format_errors(std::cerr))
    return 1;
  if (pa->aborted)
    {
      std::cerr << "--ABORT-- read\n";
      return 1;
    }
  spot::postprocessor post;
  post.set_type(spot::postprocessor::Buchi);
  post.set_pref(spot::postprocessor::SBAcc
                | spot::postprocessor::Deterministic);
  post.set_level(spot::postprocessor::High);
  auto aut = post.run(pa->aut);
  spot::print_hoa(std::cout, aut) << '\n';
  return 0;
}
