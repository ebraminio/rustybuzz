fn main() {
    let mut build = cc::Build::new();
    build.cpp(true);
    build.flag("-std=c++11");
    build.file("harfbuzz/src/hb-aat-layout.cc");
    build.file("harfbuzz/src/hb-aat-map.cc");
    build.file("harfbuzz/src/hb-blob.cc");
    build.file("harfbuzz/src/hb-common.cc");
    build.file("harfbuzz/src/hb-face.cc");
    build.file("harfbuzz/src/hb-font.cc");
    build.file("harfbuzz/src/hb-map.cc");
    build.file("harfbuzz/src/hb-ot-face.cc");
    build.file("harfbuzz/src/hb-ot-font.cc");
    build.file("harfbuzz/src/hb-ot-layout.cc");
    build.file("harfbuzz/src/hb-ot-shape-complex-arabic.cc");
    build.file("harfbuzz/src/hb-ot-shape-complex-use.cc");
    build.file("harfbuzz/src/hb-ot-shape-fallback.cc");
    build.file("harfbuzz/src/hb-ot-shape-normalize.cc");
    build.file("harfbuzz/src/hb-ot-shape.cc");
    build.file("harfbuzz/src/hb-set.cc");
    build.file("harfbuzz/src/hb-shape-plan.cc");
    build.file("harfbuzz/src/hb-shape.cc");
    build.file("harfbuzz/src/hb-static.cc");
    build.include("harfbuzz/src");
    build.compile("libharfbuzz.a");
}
