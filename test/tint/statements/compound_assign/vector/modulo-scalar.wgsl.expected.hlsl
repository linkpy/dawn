SKIP: FAILED


struct S {
  a : vec4<i32>,
}

@group(0) @binding(0) var<storage, read_write> v : S;

fn foo() {
  v.a %= 2;
}

Failed to generate: error: cannot assign to value of type 'vec4<i32>'
