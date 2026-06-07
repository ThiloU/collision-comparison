use std::fs;

use glam::{dvec3, DMat4, dmat4, dvec4, DVec3, DVec4};
use serde_json::Value;

use crate::{colliders::Collider};

pub fn load_test_file(path: &str) -> Vec<(Collider, Collider, f64)> {
    let contents = fs::read_to_string(path).unwrap();
    let json_data: Value = serde_json::from_str(&contents).unwrap();

    let mut result: Vec<(Collider, Collider, f64)> = Vec::new();

    for json_obj in json_data.as_array().unwrap() {

        let collider1 = parse_collider(&json_obj["collider1"]);
        let collider2 = parse_collider(&json_obj["collider2"]);
        let distance = json_obj["distance"].as_f64().unwrap();

        result.push((collider1, collider2, distance))
    }

    result
}

pub fn parse_collider(json_obj: &Value) -> Collider {
    match json_obj["type"].as_str().unwrap() {
        "Sphere" => {
            let collider2origin = parse_mat4(&json_obj["collider2origin"]);

            let radius = json_obj["radius"].as_f64().unwrap();

            Collider::new_sphere(collider2origin, radius)
        }
        "Capsule" => {
            let collider2origin = parse_mat4(&json_obj["collider2origin"]);

            let radius = json_obj["radius"].as_f64().unwrap();
            let height = json_obj["height"].as_f64().unwrap();

            Collider::new_capluse(collider2origin, radius, height)
        }   
        "Cylinder" => {
            let collider2origin = parse_mat4(&json_obj["collider2origin"]);

            let radius = json_obj["radius"].as_f64().unwrap();
            let height = json_obj["height"].as_f64().unwrap();

            Collider::new_cylinder(collider2origin, radius, height)
        }
        "Box" => {
            let collider2origin = parse_mat4(&json_obj["collider2origin"]);

            let size = parse_vec3(&json_obj["size"]);
            
            Collider::new_box(collider2origin, size)
        }
        &_ => todo!()
    }
}

fn parse_vec3(json_obj: &Value) -> DVec3 {
    dvec3(
        json_obj[0].as_f64().unwrap(),
        json_obj[1].as_f64().unwrap(),
        json_obj[2].as_f64().unwrap(),
    )
}

fn parse_vec4(json_obj: &Value) -> DVec4 {
    dvec4(
        json_obj[0].as_f64().unwrap(),
        json_obj[1].as_f64().unwrap(),
        json_obj[2].as_f64().unwrap(),
        json_obj[3].as_f64().unwrap(),
    )
}

fn parse_mat4(json_obj: &Value) -> DMat4 {
    dmat4(
        parse_vec4(&json_obj[0]),
        parse_vec4(&json_obj[1]),
        parse_vec4(&json_obj[2]),
        parse_vec4(&json_obj[3]),
    ).transpose()
}

#[cfg(test)]
mod test{
    use glam::dvec3;
    use serde_json::Value;

    use crate::{colliders::ColliderType, json_loder::{parse_collider, load_test_file}, gjk::GJKNesterov};

    #[test]
    fn test_parse_json_collider() {
        
        let json_obj: Value = serde_json::from_str(r#"
        {
            "type": "Sphere",
            "center": [
                1.0,
                0.0,
                0.0
            ],
            "radius": 10.0
        }"#).unwrap();

        let collider = parse_collider(&json_obj);
        assert!(collider.typ == ColliderType::Sphere);
        assert!(collider.center == dvec3(1.0, 0.0, 0.0));
        assert!(collider.radius == 10.0);


        let json_obj: Value = serde_json::from_str(r#"
        {
            "type": "Capsule",
            "center": [
                0.0,
                1.0,
                2.0
            ],
            "radius": 1.0,
            "height": 2.0
        }"#).unwrap();

        let collider = parse_collider(&json_obj);
        assert!(collider.typ == ColliderType::Capluse);
        // assert!(collider.center == vec3(0.0, 1.0, 2.0)); TODO
        assert!(collider.radius == 1.0);
        assert!(collider.height == 2.0);


        let json_obj: Value = serde_json::from_str(r#"
        {
            "type": "Cylinder",
            "center": [
                5.0,
                1.0,
                0.0
            ],
            "radius": 10.0,
            "height": 3.0
        }"#).unwrap();

        let collider = parse_collider(&json_obj);
        assert!(collider.typ == ColliderType::Cylinder);
        // assert!(collider.center == vec3(5.0, 1.0, 0.0)); TODO
        assert!(collider.radius == 10.0);
        assert!(collider.height == 3.0);
    }
}

