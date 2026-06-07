use glam::{DVec3, DMat4, Vec4Swizzles, DMat3};

pub mod random;
pub mod support_point;

#[derive(PartialEq, Clone, Copy)]
pub enum ColliderType {
    Sphere,
    Capluse,
    Cylinder,
    Box,
}

pub struct Collider {
    pub typ: ColliderType,

    pub transform: DMat3,
    pub transform_transposed: DMat3,
    pub center: DVec3,

    pub radius: f64,
    pub height: f64,
    pub size: DVec3,
}

impl Collider {

    pub fn new_sphere(collider2origin: DMat4, radius: f64) -> Self {
        let transform = DMat3::from_mat4(collider2origin);

        Self { 
            typ: ColliderType::Sphere, 
            transform, 
            transform_transposed: transform.transpose(),
            center: Self::get_center_from_collider2origin(&collider2origin), 
            radius: radius, 
            height: 0.0,
            size: DVec3::ZERO,
        }
    }

    pub fn new_capluse(collider2origin: DMat4, radius: f64, height: f64) -> Self {
        let transform = DMat3::from_mat4(collider2origin);

        Self { 
            typ: ColliderType::Capluse, 
            transform, 
            transform_transposed: transform.transpose(),
            center: Self::get_center_from_collider2origin(&collider2origin), 
            radius: radius, 
            height: height,
            size: DVec3::ZERO,
        }
    }

    pub fn new_cylinder(collider2origin: DMat4, radius: f64, height: f64) -> Self {
        let transform = DMat3::from_mat4(collider2origin);

        Self { 
            typ: ColliderType::Cylinder, 
            transform, 
            transform_transposed: transform.transpose(),
            center: Self::get_center_from_collider2origin(&collider2origin), 
            radius: radius, 
            height: height,
            size: DVec3::ZERO,
        }
    }

    pub fn new_box(collider2origin: DMat4, size: DVec3) -> Self {
        let transform = DMat3::from_mat4(collider2origin);

        Self { 
            typ: ColliderType::Box, 
            transform, 
            transform_transposed: transform.transpose(),
            center: Self::get_center_from_collider2origin(&collider2origin), 
            radius: 0.0, 
            height: 0.0,
            size: size,
        }
    }

    fn get_center_from_collider2origin(collider2origin: &DMat4 ) -> DVec3 {
        collider2origin.w_axis.xyz()
    }

}