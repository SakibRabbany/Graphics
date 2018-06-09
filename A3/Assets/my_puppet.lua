-- my_puppet.lua
-- simple humanoid model

rootnode = gr.node('root')
    rootnode:rotate('y', 0.0)
    rootnode:scale( 0.25, 0.25, 0.25 )
    rootnode:translate(0.0, 0.0, -1.0)



red = gr.material({1.0, 0.0, 0.0}, {0.1, 0.1, 0.1}, 10)
blue = gr.material({0.0, 0.0, 1.0}, {0.1, 0.1, 0.1}, 10)
green = gr.material({0.0, 1.0, 0.0}, {0.1, 0.1, 0.1}, 10)
white = gr.material({1.0, 1.0, 1.0}, {0.1, 0.1, 0.1}, 10)

torso_height = 1.0
torso_width = 0.5

body_width = 0.4

shoulder_height = 0.18
shoulder_width = 1.4

hip_height = 0.15
hip_width = 1.2

upper_arm_height = 3.8
upper_arm_width = 0.23

fore_arm_height = 0.9
fore_arm_width = 0.8

hand_height = 0.3
hand_width = 1.6

thigh_height = 5
thigh_width = 0.35

calf_height = 0.9
calf_width = 0.8

foot_height = 0.13
foot_width = 2


-- torso definition
-- parent: root
torso = gr.mesh('cube', 'torso')
    rootnode:add_child(torso)
    torso:set_material(blue)
    torso:scale(torso_width,torso_height,body_width);

-- shoulder definition
-- parent: torso
shoulders = gr.mesh('cube', 'shoulders')
    torso:add_child(shoulders)
    shoulders:set_material(green)
    shoulders:scale(shoulder_width, shoulder_height, body_width*2+0.2);
    shoulders:translate(0, torso_height/2 + shoulder_height/2, 0)

jointnode = gr.mesh('sphere', 'joint')

-- hip definition
-- parent: torso
hips = gr.mesh('cube', 'hips')
    torso:add_child(hips)
    hips:set_material(green)
    hips:scale(hip_width, hip_height, body_width*2+0.2);
    hips:translate(0, -(torso_height/2 + hip_height/2), 0)

-- right upper arm definition
-- parent:shoulder
right_upper_arm = gr.mesh('cube', 'right_upper_arm')
    shoulders:add_child(right_upper_arm)
    right_upper_arm:set_material(red)
    right_upper_arm:scale(upper_arm_width, upper_arm_height, upper_arm_width+0.17 )
    right_upper_arm:translate(-(shoulder_width/2-0.1), -(shoulder_height/2 + upper_arm_height/2)+0.4, 0)


-- left upper arm definition
-- parent:shoulder
left_upper_arm = gr.mesh('cube', 'left_upper_arm')
    shoulders:add_child(left_upper_arm)
    left_upper_arm:set_material(red)
    left_upper_arm:scale(upper_arm_width, upper_arm_height, upper_arm_width+0.17)
    left_upper_arm:translate(shoulder_width/2-0.1, -(shoulder_height/2 + upper_arm_height/2)+0.4, 0)


-- left lower arm definition
-- parent: left upper arm
left_fore_arm = gr.mesh('cube', 'left_fore_arm')
    left_upper_arm:add_child(left_fore_arm)
    left_fore_arm:set_material(blue)
    left_fore_arm:scale(fore_arm_width, fore_arm_height, fore_arm_width)
    left_fore_arm:translate(0,-(upper_arm_height/5+0.19),0)

-- right lower arm definition
-- parent: right upper arm
right_fore_arm = gr.mesh('cube', 'right_fore_arm')
    right_upper_arm:add_child(right_fore_arm)
    right_fore_arm:set_material(blue)
    right_fore_arm:scale(fore_arm_width, fore_arm_height, fore_arm_width)
    right_fore_arm:translate(0,-(upper_arm_height/5+0.19),0)

-- right hand definition
-- parent: right fore arm
right_hand = gr.mesh('cube', 'right_hand')
    right_fore_arm:add_child(right_hand)
    right_hand:set_material(green)
    right_hand:scale(hand_width, hand_height, 1)
    right_hand:translate(0, -(fore_arm_height/2 + hand_height/2), 0)


-- left hand definition
-- parent: left fore arm
left_hand = gr.mesh('cube', 'left_hand')
    left_fore_arm:add_child(left_hand)
    left_hand:set_material(green)
    left_hand:scale(hand_width, hand_height, 1)
    left_hand:translate(0, -(fore_arm_height/2 + hand_height/2), 0)


-- right thigh definition
-- parent: hips
right_thigh = gr.mesh('cube', 'right_thigh')
    hips:add_child(right_thigh)
    right_thigh:set_material(red)
    right_thigh:scale(thigh_width, thigh_height, thigh_width +0.2)
    right_thigh:translate(-(hip_width/4 + thigh_width/4 - 0.1), -(hip_height/2 + thigh_height/2 + 0.35), 0)


-- left thigh definition
-- parent: hips
left_thigh = gr.mesh('cube', 'left_thigh')
    hips:add_child(left_thigh)
    left_thigh:set_material(red)
    left_thigh:scale(thigh_width, thigh_height, thigh_width + 0.2 )
    left_thigh:translate((hip_width/4 + thigh_width/4 - 0.1), -(hip_height/2 + thigh_height/2 + 0.35), 0)

-- right calf definition
-- parent: right thigh
right_calf = gr.mesh('cube', 'right_calf')
    right_thigh:add_child(right_calf)
    right_calf:set_material(blue)
    right_calf:scale(calf_width, calf_height, calf_width)
    right_calf:translate(0, -0.9, 0)

-- left calf definition
-- parent: left thigh
left_calf = gr.mesh('cube', 'left_calf')
    left_thigh:add_child(left_calf)
    left_calf:set_material(blue)
    left_calf:scale(calf_width, calf_height, calf_width)
    left_calf:translate(0, -0.9, 0)

-- right foot definition
-- parent: right calf
right_foot = gr.mesh('cube', 'right_foot')
    right_calf:add_child(right_foot)
    right_foot:set_material(green)
    right_foot:scale(1, foot_height, foot_width)
    right_foot:translate(0, -0.52, 0.5)

-- left foot definition
-- parent: left calf
left_foot = gr.mesh('cube', 'left_foot')
    left_calf:add_child(left_foot)
    left_foot:set_material(green)
    left_foot:scale(1, foot_height, foot_width)
    left_foot:translate(0, -0.52, 0.5)



return rootnode
