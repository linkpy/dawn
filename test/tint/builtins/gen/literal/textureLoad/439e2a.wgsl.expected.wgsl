@group(1) @binding(0) var arg_0 : texture_2d<f32>;

fn textureLoad_439e2a() {
  var res : vec4<f32> = textureLoad(arg_0, vec2<u32>(), 1i);
}

@vertex
fn vertex_main() -> @builtin(position) vec4<f32> {
  textureLoad_439e2a();
  return vec4<f32>();
}

@fragment
fn fragment_main() {
  textureLoad_439e2a();
}

@compute @workgroup_size(1)
fn compute_main() {
  textureLoad_439e2a();
}