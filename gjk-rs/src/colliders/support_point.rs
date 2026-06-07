use glam::{DVec3, dvec3};

use super::{Collider, ColliderType};


impl Collider {
    pub fn get_support_point(&self, dir: DVec3) -> DVec3 {
        match self.typ {
            ColliderType::Sphere => {
                dir.normalize() * self.radius + self.center
            },
    
            ColliderType::Capluse => {
                /*
                You can find similar implementations here:
    
                * https://github.com/kevinmoran/GJK/blob/b38d923d268629f30b44c3cf6d4f9974bbcdb0d3/Collider.h#L57
                (Copyright (c) 2017 Kevin Moran, MIT License or Unlicense)
                * https://github.com/bulletphysics/bullet3/blob/e306b274f1885f32b7e9d65062aa942b398805c2/src/BulletCollision/CollisionShapes/btConvexShape.cpp#L228
                (Copyright (c) 2003-2009 Erwin Coumans, zlib license)
                */
                let local_dir = self.transform_transposed * dir;

                let s = (local_dir.x * local_dir.x + local_dir.y * local_dir.y + local_dir.z * local_dir.z).sqrt();
    
                let mut local_vertex = if s == 0.0 { 
                    dvec3(self.radius, 0.0, 0.0) 
                } else {  
                    local_dir * (self.radius / s)
                };

                local_vertex.z += if local_vertex.z > 0.0 { 
                    0.5 * self.height 
                } else { 
                    -0.5 * self.height 
                };

                self.center + (self.transform * local_vertex)
            },
    
            ColliderType::Cylinder => {
                /*
                You can find similar implementations here:
    
                * https://github.com/kevinmoran/GJK/blob/b38d923d268629f30b44c3cf6d4f9974bbcdb0d3/Collider.h#L42
                (Copyright (c) 2017 Kevin Moran, MIT License or Unlicense)
                * https://github.com/bulletphysics/bullet3/blob/e306b274f1885f32b7e9d65062aa942b398805c2/src/BulletCollision/CollisionShapes/btConvexShape.cpp#L167
                (Copyright (c) 2003-2009 Erwin Coumans, zlib license) 
                */
                let local_dir =  self.transform_transposed * dir;

                let s = (local_dir.x * local_dir.x + local_dir.y * local_dir.y).sqrt();

                let z = if local_dir.z < 0.0 { -0.5 * self.height } else { 0.5 * self.height };
                
                let local_vertex = if s == 0.0 { 
                    dvec3(self.radius, 0.0, z) 
                } else {  
                    let d = self.radius / s;
                    dvec3(local_dir.x * d, local_dir.y * d, z) 
                };

                self.center + (self.transform * local_vertex)
            },    
            ColliderType::Box => {
                /*
                You can find similar implementations here:
    
                * https://github.com/kevinmoran/GJK/blob/b38d923d268629f30b44c3cf6d4f9974bbcdb0d3/Collider.h#L42
                (Copyright (c) 2017 Kevin Moran, MIT License or Unlicense)
                * https://github.com/bulletphysics/bullet3/blob/e306b274f1885f32b7e9d65062aa942b398805c2/src/BulletCollision/CollisionShapes/btConvexShape.cpp#L167
                (Copyright (c) 2003-2009 Erwin Coumans, zlib license) 
                */
                
                let local_dir = self.transform_transposed * dir;

                let mut local_vertex = self.size * 0.5;
                if local_dir.x < 0.0 {
                    local_vertex.x *= -1.0;
                }
                if local_dir.y < 0.0 {
                    local_vertex.y *= -1.0;
                }
                if local_dir.z < 0.0 {
                    local_vertex.z *= -1.0;
                }

                self.center + (self.transform * local_vertex)
            },    
        }
    }
}