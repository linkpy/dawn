struct buf0 {
  resolution : vec2<f32>;
}

var<private> gl_FragCoord : vec4<f32>;

@group(0) @binding(0) var<uniform> x_6 : buf0;

var<private> x_GLF_color : vec4<f32>;

fn main_1() {
  var x_196 : array<i32, 256u>;
  var x_197 : array<i32, 256u>;
  var x_198 : array<i32, 256u>;
  var x_210 : vec2<i32>;
  var x_249 : vec4<f32>;
  var x_251 : vec4<f32>;
  var x_214_phi : vec2<i32>;
  var x_251_phi : vec4<f32>;
  var x_252_phi : bool;
  var x_254_phi : vec4<f32>;
  let x_199 : vec4<f32> = gl_FragCoord;
  let x_202 : vec2<f32> = x_6.resolution;
  let x_203 : vec2<f32> = (vec2<f32>(x_199.x, x_199.y) / x_202);
  x_210 = vec2<i32>(i32((x_203.x * 256.0)), i32((x_203.y * 256.0)));
  switch(0u) {
    default: {
      x_214_phi = x_210;
      loop {
        var x_233 : i32;
        var x_234 : i32;
        var x_235_phi : i32;
        let x_214 : vec2<i32> = x_214_phi;
        let x_217 : i32 = x_214.y;
        x_251_phi = vec4<f32>();
        x_252_phi = false;
        if ((x_217 != 256)) {
        } else {
          break;
        }
        let x_221 : i32 = x_214.x;
        x_196 = array<i32, 256u>(115, 133, 150, 164, 176, 184, 190, 192, 191, 187, 181, 172, 163, 153, 143, 134, 126, 120, 116, 114, 114, 117, 121, 127, 134, 141, 148, 154, 159, 162, 163, 161, 157, 151, 143, 134, 124, 113, 103, 94, 87, 82, 79, 80, 84, 91, 101, 114, 130, 146, 164, 182, 199, 215, 229, 240, 249, 254, 256, 254, 250, 243, 233, 223, 212, 200, 190, 180, 172, 166, 163, 161, 162, 164, 169, 174, 179, 185, 190, 193, 195, 195, 192, 188, 180, 171, 161, 149, 137, 125, 114, 105, 97, 93, 91, 93, 98, 106, 117, 130, 145, 161, 177, 193, 208, 221, 231, 239, 243, 244, 242, 236, 228, 218, 207, 194, 181, 169, 158, 148, 141, 135, 132, 131, 132, 135, 138, 143, 147, 151, 154, 155, 155, 152, 146, 139, 129, 118, 106, 93, 80, 68, 58, 49, 43, 40, 41, 44, 51, 61, 73, 87, 103, 119, 134, 149, 162, 173, 181, 186, 188, 186, 181, 174, 164, 153, 141, 128, 116, 104, 94, 86, 81, 77, 76, 77, 80, 84, 89, 94, 98, 102, 104, 104, 102, 98, 92, 83, 73, 62, 50, 38, 26, 16, 8, 2, 0, 0, 4, 11, 21, 33, 48, 64, 81, 98, 114, 129, 141, 151, 158, 161, 161, 158, 152, 144, 134, 123, 112, 100, 90, 81, 73, 68, 65, 65, 67, 70, 75, 81, 87, 92, 97, 101, 103, 102, 100, 95, 88, 79, 69, 58, 47, 36, 26, 18, 13, 11, 11, 15, 22, 32, 45, 60, 77, 94);
        let x_223 : i32 = x_196[x_217];
        let x_225 : bool = (x_221 < (x_223 + 15));
        if (x_225) {
          x_197 = array<i32, 256u>(115, 133, 150, 164, 176, 184, 190, 192, 191, 187, 181, 172, 163, 153, 143, 134, 126, 120, 116, 114, 114, 117, 121, 127, 134, 141, 148, 154, 159, 162, 163, 161, 157, 151, 143, 134, 124, 113, 103, 94, 87, 82, 79, 80, 84, 91, 101, 114, 130, 146, 164, 182, 199, 215, 229, 240, 249, 254, 256, 254, 250, 243, 233, 223, 212, 200, 190, 180, 172, 166, 163, 161, 162, 164, 169, 174, 179, 185, 190, 193, 195, 195, 192, 188, 180, 171, 161, 149, 137, 125, 114, 105, 97, 93, 91, 93, 98, 106, 117, 130, 145, 161, 177, 193, 208, 221, 231, 239, 243, 244, 242, 236, 228, 218, 207, 194, 181, 169, 158, 148, 141, 135, 132, 131, 132, 135, 138, 143, 147, 151, 154, 155, 155, 152, 146, 139, 129, 118, 106, 93, 80, 68, 58, 49, 43, 40, 41, 44, 51, 61, 73, 87, 103, 119, 134, 149, 162, 173, 181, 186, 188, 186, 181, 174, 164, 153, 141, 128, 116, 104, 94, 86, 81, 77, 76, 77, 80, 84, 89, 94, 98, 102, 104, 104, 102, 98, 92, 83, 73, 62, 50, 38, 26, 16, 8, 2, 0, 0, 4, 11, 21, 33, 48, 64, 81, 98, 114, 129, 141, 151, 158, 161, 161, 158, 152, 144, 134, 123, 112, 100, 90, 81, 73, 68, 65, 65, 67, 70, 75, 81, 87, 92, 97, 101, 103, 102, 100, 95, 88, 79, 69, 58, 47, 36, 26, 18, 13, 11, 11, 15, 22, 32, 45, 60, 77, 94);
        }
        if (x_225) {
          x_233 = x_197[x_217];
          x_235_phi = x_233;
        } else {
          x_234 = 0;
          x_235_phi = x_234;
        }
        let x_235 : i32 = x_235_phi;
        let x_237 : bool = (x_221 > (x_235 - 15));
        if (select(x_225, x_237, x_225)) {
          x_198 = array<i32, 256u>(115, 133, 150, 164, 176, 184, 190, 192, 191, 187, 181, 172, 163, 153, 143, 134, 126, 120, 116, 114, 114, 117, 121, 127, 134, 141, 148, 154, 159, 162, 163, 161, 157, 151, 143, 134, 124, 113, 103, 94, 87, 82, 79, 80, 84, 91, 101, 114, 130, 146, 164, 182, 199, 215, 229, 240, 249, 254, 256, 254, 250, 243, 233, 223, 212, 200, 190, 180, 172, 166, 163, 161, 162, 164, 169, 174, 179, 185, 190, 193, 195, 195, 192, 188, 180, 171, 161, 149, 137, 125, 114, 105, 97, 93, 91, 93, 98, 106, 117, 130, 145, 161, 177, 193, 208, 221, 231, 239, 243, 244, 242, 236, 228, 218, 207, 194, 181, 169, 158, 148, 141, 135, 132, 131, 132, 135, 138, 143, 147, 151, 154, 155, 155, 152, 146, 139, 129, 118, 106, 93, 80, 68, 58, 49, 43, 40, 41, 44, 51, 61, 73, 87, 103, 119, 134, 149, 162, 173, 181, 186, 188, 186, 181, 174, 164, 153, 141, 128, 116, 104, 94, 86, 81, 77, 76, 77, 80, 84, 89, 94, 98, 102, 104, 104, 102, 98, 92, 83, 73, 62, 50, 38, 26, 16, 8, 2, 0, 0, 4, 11, 21, 33, 48, 64, 81, 98, 114, 129, 141, 151, 158, 161, 161, 158, 152, 144, 134, 123, 112, 100, 90, 81, 73, 68, 65, 65, 67, 70, 75, 81, 87, 92, 97, 101, 103, 102, 100, 95, 88, 79, 69, 58, 47, 36, 26, 18, 13, 11, 11, 15, 22, 32, 45, 60, 77, 94);
          let x_243 : i32 = x_198[x_217];
          let x_248 : f32 = ((15.0 - abs(f32((x_221 - x_243)))) * 0.06666667);
          x_249 = vec4<f32>(x_248, x_248, x_248, 1.0);
          x_251_phi = x_249;
          x_252_phi = true;
          break;
        }
        var x_215_1 : vec2<i32> = x_214;
        x_215_1.y = (x_217 + 1);
        let x_215 : vec2<i32> = x_215_1;

        continuing {
          x_214_phi = x_215;
        }
      }
      x_251 = x_251_phi;
      let x_252 : bool = x_252_phi;
      x_254_phi = x_251;
      if (x_252) {
        break;
      }
      x_254_phi = vec4<f32>(0.0, 0.0, 0.0, 1.0);
    }
  }
  let x_254 : vec4<f32> = x_254_phi;
  x_GLF_color = x_254;
  return;
}

struct main_out {
  @location(0)
  x_GLF_color_1 : vec4<f32>;
}

@stage(fragment)
fn main(@builtin(position) gl_FragCoord_param : vec4<f32>) -> main_out {
  gl_FragCoord = gl_FragCoord_param;
  main_1();
  return main_out(x_GLF_color);
}
