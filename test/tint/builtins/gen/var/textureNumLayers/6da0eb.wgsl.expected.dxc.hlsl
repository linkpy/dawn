SKIP: FAILED


enable chromium_experimental_read_write_storage_texture;

@group(1) @binding(0) var arg_0 : texture_storage_2d_array<rgba8unorm, read_write>;

fn textureNumLayers_6da0eb() {
  var res : u32 = textureNumLayers(arg_0);
  prevent_dce = res;
}

@group(2) @binding(0) var<storage, read_write> prevent_dce : u32;

@vertex
fn vertex_main() -> @builtin(position) vec4<f32> {
  textureNumLayers_6da0eb();
  return vec4<f32>();
}

@fragment
fn fragment_main() {
  textureNumLayers_6da0eb();
}

@compute @workgroup_size(1)
fn compute_main() {
  textureNumLayers_6da0eb();
}

Failed to generate: builtins/gen/var/textureNumLayers/6da0eb.wgsl:24:8 error: HLSL backend does not support extension 'chromium_experimental_read_write_storage_texture'
enable chromium_experimental_read_write_storage_texture;
       ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

