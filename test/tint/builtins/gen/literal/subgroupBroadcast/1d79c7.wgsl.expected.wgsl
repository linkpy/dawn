SKIP: FAILED

enable chromium_experimental_subgroups;

fn subgroupBroadcast_1d79c7() {
  var res : i32 = subgroupBroadcast(1i, 1u);
  prevent_dce = res;
}

@group(2) @binding(0) var<storage, read_write> prevent_dce : i32;

@compute @workgroup_size(1)
fn compute_main() {
  subgroupBroadcast_1d79c7();
}
