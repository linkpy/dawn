SKIP: FAILED


enable chromium_experimental_read_write_storage_texture;

@group(1) @binding(0) var arg_0 : texture_storage_2d_array<rg32sint, read_write>;

fn textureNumLayers_622aa2() {
  var res : u32 = textureNumLayers(arg_0);
  prevent_dce = res;
}

@group(2) @binding(0) var<storage, read_write> prevent_dce : u32;

@vertex
fn vertex_main() -> @builtin(position) vec4<f32> {
  textureNumLayers_622aa2();
  return vec4<f32>();
}

@fragment
fn fragment_main() {
  textureNumLayers_622aa2();
}

@compute @workgroup_size(1)
fn compute_main() {
  textureNumLayers_622aa2();
}

Failed to generate: builtins/gen/literal/textureNumLayers/622aa2.wgsl:24:8 error: HLSL backend does not support extension 'chromium_experimental_read_write_storage_texture'
enable chromium_experimental_read_write_storage_texture;
       ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

