// using namespace jules::formula_literals;
// model:     "a"_v, "b"_v, "c"_v          == var<>("a"), var<>("c"), var<>("c")
// formula:   ("a"_v, "b"_v, "c"_v)->("y") == (var<>("a"), var<>("c"), var<>("c"))->(var<>("y"))

// model:   "a"_nv, "b"_nv, "c"_nv             == var<numeric>("a"), var<numeric>("c"), var<numeric>("c")
// formula: ("a"_nv, "b"_nv, "c"_nv)->("y"_nv) == (var<numeric>("a"), var<numeric>("c"), var<numeric>("c"))->(var<numeric>("y"))
