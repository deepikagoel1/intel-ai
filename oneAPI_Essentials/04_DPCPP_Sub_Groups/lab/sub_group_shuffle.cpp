
//==============================================================
// Copyright © 2020 Intel Corporation
//
// SPDX-License-Identifier: MIT
// =============================================================
#include <CL/sycl.hpp>
using namespace sycl;

static constexpr size_t N = 256; // global size
static constexpr size_t B = 64;  // work-group size

int main() {
  queue q;
  std::cout << "Device : " << q.get_device().get_info<info::device::name>() << std::endl;

  //# initialize data array using usm
  int *data = malloc_shared<int>(N, q);
  for (int i = 0; i < N; i++) data[i] = i;
  for (int i = 0; i < N; i++) std::cout << data[i] << " ";
  std::cout << std::endl << std::endl;

  q.parallel_for(nd_range<1>(N, B), [=](nd_item<1> item) {
    auto sg = item.get_sub_group();
    auto i = item.get_global_id(0);

    //# swap adjacent items in array using sub_group shuffle_xor
    data[i] = sg.shuffle_xor(data[i], 1);

    //# reverse the order of items in sub_group using shuffle_xor
    //data[i] = sg.shuffle_xor(data[i], sg.get_max_local_range() - 1);

  }).wait();

  for (int i = 0; i < N; i++) std::cout << data[i] << " ";
  free(data, q);
  return 0;
}
