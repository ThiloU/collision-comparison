import pytransform3d.visualizer as pv

from src import set_random_joints, complex_env_dual_arm_offset, get_complex_env_bvh, get_dual_arm_tm, \
    get_dual_arm_bvh_from_tm

use_visual_meshes = True

complex_env_tm, complex_env_bvh = get_complex_env_bvh(use_visuals=use_visual_meshes)
dual_arm_tm = get_dual_arm_tm()
set_random_joints(dual_arm_tm)
dual_arm_bvh = get_dual_arm_bvh_from_tm(dual_arm_tm, base_frame2origin=complex_env_dual_arm_offset, use_visuals=use_visual_meshes)



fig = pv.figure()

for artist in complex_env_bvh.get_artists():
    artist.add_artist(fig)

for artist in dual_arm_bvh.get_artists():
    artist.add_artist(fig)

if "__file__" in globals():
    fig.show()
else:
    fig.save_image("__open3d_rendered_image.jpg")

# show all colliders which are potentially colliding:
# colliding_colliders = complex_env_bvh.aabb_overlapping_with_other_bvh(dual_arm_bvh)
# fig = pv.figure()
# for collider in colliding_colliders:
#     collider[0][1].artist_.add_artist(fig)
#     collider[1][1].artist_.add_artist(fig)
#
# fig.show()