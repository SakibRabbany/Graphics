-- my_puppet.lua
-- simple humanoid model

rootnode = gr.node('root')
    rootnode:rotate('y', 0.0)
    rootnode:scale( 0.25, 0.25, 0.25 )
    rootnode:translate(0.0, 0.0, -1.0)

joint_angle = {-25, 0, 25}

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
    right_upper_arm:set_material(red)
    right_upper_arm:scale(upper_arm_width/0.25, upper_arm_height, (upper_arm_width+0.17)/0.4 )
    right_upper_arm:translate(0, -(shoulder_height/2 + upper_arm_height/2)+0.4, 0)


-- left upper arm definition
-- parent:shoulder
left_upper_arm = gr.mesh('cube', 'left_upper_arm')
    left_upper_arm:set_material(red)
    left_upper_arm:scale(upper_arm_width/0.25, upper_arm_height, (upper_arm_width+0.17)/0.4)
    left_upper_arm:translate(0, -(shoulder_height/2 + upper_arm_height/2)+0.4, 0)


-- left lower arm definition
-- parent: left upper arm
left_fore_arm = gr.mesh('cube', 'left_fore_arm')
    left_fore_arm:set_material(blue)
    left_fore_arm:scale(fore_arm_width/0.75, fore_arm_height/0.2, fore_arm_width/0.75)
    left_fore_arm:translate(0,-(upper_arm_height/5+1.5),0)

-- right lower arm definition
-- parent: right upper arm
right_fore_arm = gr.mesh('cube', 'right_fore_arm')
    right_fore_arm:set_material(blue)
    right_fore_arm:scale(fore_arm_width/0.75, fore_arm_height/0.2, fore_arm_width/0.75)
    right_fore_arm:translate(0,-(upper_arm_height/5+1.5),0)

-- right hand definition
-- parent: right fore arm
right_hand = gr.mesh('cube', 'right_hand')
    right_hand:set_material(green)
    right_hand:scale(hand_width/0.75, hand_height/0.175, 1/0.75)
    right_hand:translate(0, -(fore_arm_height/2 + hand_height/2), 0)


-- left hand definition
-- parent: left fore arm
left_hand = gr.mesh('cube', 'left_hand')
    left_hand:set_material(green)
    left_hand:scale(hand_width/0.75, hand_height/0.175, 1/0.75)
    left_hand:translate(0, -(fore_arm_height/2 + hand_height/2), 0)


-- right thigh definition
-- parent: hips
right_thigh = gr.mesh('cube', 'right_thigh')
    right_thigh:set_material(red)
    right_thigh:scale(thigh_width/0.3, thigh_height, (thigh_width + 0.2)/0.33 )
    right_thigh:translate(0, -(hip_height/2 + thigh_height/2 + 0.35), 0)


-- left thigh definition
-- parent: hips
left_thigh = gr.mesh('cube', 'left_thigh')
    left_thigh:set_material(red)
    left_thigh:scale(thigh_width/0.3, thigh_height, (thigh_width + 0.2)/0.33 )
    left_thigh:translate(0, -(hip_height/2 + thigh_height/2 + 0.35), 0)

-- right calf definition
-- parent: right thigh
right_calf = gr.mesh('cube', 'right_calf')
    right_calf:set_material(blue)
    right_calf:scale(calf_width/0.6, calf_height/0.2, calf_width/0.6)
    right_calf:translate(0, -2, 0)

-- left calf definition
-- parent: left thigh
left_calf = gr.mesh('cube', 'left_calf')
    left_calf:set_material(blue)
    left_calf:scale(calf_width/0.6, calf_height/0.2, calf_width/0.6)
    left_calf:translate(0, -2, 0)

-- right foot definition
-- parent: right calf
right_foot = gr.mesh('cube', 'right_foot')
    right_foot:set_material(green)
    right_foot:scale(1/0.6, foot_height/0.2, foot_width/0.6)
    right_foot:translate(0, -0.4, 0.7)

-- left foot definition
-- parent: left calf
left_foot = gr.mesh('cube', 'left_foot')
    left_foot:set_material(green)
    left_foot:scale(1/0.6, foot_height/0.2, foot_width/0.6)
    left_foot:translate(0, -0.4, 0.7)

-- neck definition
neck = gr.mesh('cube', 'neck')
    shoulders:add_child(neck)
    neck:set_material(blue)
    neck:scale(0.2, 1.1, 0.5)
    neck:translate(0, 1.1, 0)

-- head definition
head = gr.mesh('cube', 'head')
    head:set_material(red)
    head:scale(2.6/0.8, 1.7/0.5, 2.1/0.6)
    head:translate(0, 1.1, 0)

ears = gr.mesh('cube', 'ears')
    head:add_child(ears)
    ears:set_material(blue)
    ears:scale(1.2, 0.25, 0.25)
    ears:translate(0, 0.1, -0.1)

left_eye = gr.mesh('cube', 'left_eye')
    head:add_child(left_eye)
    left_eye:set_material(blue)
    left_eye:scale(0.25, 0.15, 0.25)
    left_eye:translate(0.2, 0.2, 0.5)

right_eye = gr.mesh('cube', 'right_eye')
    head:add_child(right_eye)
    right_eye:set_material(blue)
    right_eye:scale(0.25, 0.15, 0.25)
    right_eye:translate(-0.2, 0.2, 0.5)

ears = gr.mesh('cube', 'ears')
    head:add_child(ears)
    ears:set_material(blue)
    ears:scale(1.2, 0.25, 0.25)
    ears:translate(0, 0.1, -0.1)



-- joints

-- left shoulder
left_shoulder = gr.mesh('sphere', 'left_shoulder')
shoulders:add_child(left_shoulder)
left_shoulder:set_material(green)
left_shoulder:scale(0.25, 1, 0.4)

left_shoulder:translate(shoulder_width/2-0.1, 0, 0)

left_shoulder_joint = gr.joint('left_shoulder_joint', joint_angle, joint_angle)
left_shoulder:add_child(left_shoulder_joint)
left_shoulder_joint:add_child(left_upper_arm)

-- right shoulder
right_shoulder = gr.mesh('sphere', 'right_shoulder')
shoulders:add_child(right_shoulder)
right_shoulder:set_material(green)
right_shoulder:scale(0.25, 1, 0.4)
right_shoulder:translate(-(shoulder_width/2-0.1), 0, 0)

right_shoulder_joint = gr.joint('right_shoulder_joint', joint_angle, joint_angle)
right_shoulder:add_child(right_shoulder_joint)
right_shoulder_joint:add_child(right_upper_arm)

-- left elbow
left_elbow = gr.mesh('sphere', 'left_elbow')
left_upper_arm:add_child(left_elbow)
left_elbow:set_material(green)
left_elbow:scale(0.75, 0.2, 0.75)
left_elbow:translate(0, -0.47, 0)

left_elbow_joint = gr.joint('left_elbow_joint', joint_angle, joint_angle)
left_elbow:add_child(left_elbow_joint)
left_elbow_joint:add_child(left_fore_arm)


-- right elbow
right_elbow = gr.mesh('sphere', 'right_elbow')
right_upper_arm:add_child(right_elbow)
right_elbow:set_material(green)
right_elbow:scale(0.75, 0.2, 0.75)
right_elbow:translate(0, -0.47, 0)

right_elbow_joint = gr.joint('right_elbow_joint', joint_angle, joint_angle)
right_elbow:add_child(right_elbow_joint)
right_elbow_joint:add_child(right_fore_arm)


-- left wrist
left_wrist = gr.mesh('sphere', 'left_wrist')
left_fore_arm:add_child(left_wrist)
left_wrist:set_material(green)
left_wrist:scale(0.75, 0.175, 0.75)
left_wrist:translate(0, -0.47, 0)

left_wrist_joint = gr.joint('left_wrist_joint', joint_angle, joint_angle)
left_wrist:add_child(left_wrist_joint)
left_wrist_joint:add_child(left_hand)


-- right wrist
right_wrist = gr.mesh('sphere', 'right_wrist')
right_fore_arm:add_child(right_wrist)
right_wrist:set_material(green)
right_wrist:scale(0.75, 0.175, 0.75)
right_wrist:translate(0, -0.47, 0)

right_wrist_joint = gr.joint('right_wrist_joint', joint_angle, joint_angle)
right_wrist:add_child(right_wrist_joint)
right_wrist_joint:add_child(right_hand)


-- left hip
left_hip = gr.mesh('sphere', 'left_hip')
hips:add_child(left_hip)
left_hip:set_material(green)
left_hip:scale(0.3, 1, 0.33)
left_hip:translate((hip_width/4 + thigh_width/4 - 0.1), 0, 0)

left_hip_joint = gr.joint('left_hip_joint', joint_angle, joint_angle)
left_hip:add_child(left_hip_joint)
left_hip_joint:add_child(left_thigh)


-- right hip
right_hip = gr.mesh('sphere', 'right_hip')
hips:add_child(right_hip)
right_hip:set_material(green)
right_hip:scale(0.3, 1, 0.33)
right_hip:translate(-(hip_width/4 + thigh_width/4 - 0.1), 0, 0)

right_hip_joint = gr.joint('right_hip_joint', joint_angle, joint_angle)
right_hip:add_child(right_hip_joint)
right_hip_joint:add_child(right_thigh)


-- left knee
left_knee = gr.mesh('sphere', 'left_knee')
left_thigh:add_child(left_knee)
left_knee:set_material(green)
left_knee:scale(0.6, 0.2, 0.6)
left_knee:translate(0, -0.5, 0)

left_knee_joint = gr.joint('left_knee_joint', joint_angle, joint_angle)
left_knee:add_child(left_knee_joint)
left_knee_joint:add_child(left_calf)

-- right knee
right_knee = gr.mesh('sphere', 'right_knee')
right_thigh:add_child(right_knee)
right_knee:set_material(green)
right_knee:scale(0.6, 0.2, 0.6)
right_knee:translate(0, -0.5, 0)

right_knee_joint = gr.joint('right_knee_joint', joint_angle, joint_angle)
right_knee:add_child(right_knee_joint)
right_knee_joint:add_child(right_calf)


-- left ankle
left_ankle = gr.mesh('sphere', 'left_ankle')
left_calf:add_child(left_ankle)
left_ankle:set_material(green)
left_ankle:scale(0.6, 0.2, 0.6)
left_ankle:translate(0, -0.5, 0)

left_ankle_joint = gr.joint('left_ankle_joint', joint_angle, joint_angle)
left_ankle:add_child(left_ankle_joint)
left_ankle_joint:add_child(left_foot)


-- right ankle
right_ankle = gr.mesh('sphere', 'right_ankle')
right_calf:add_child(right_ankle)
right_ankle:set_material(green)
right_ankle:scale(0.6, 0.2, 0.6)
right_ankle:translate(0, -0.5, 0)

right_ankle_joint = gr.joint('right_ankle_joint', joint_angle, joint_angle)
right_ankle:add_child(right_ankle_joint)
right_ankle_joint:add_child(right_foot)

-- neck joint

neck_mesh = gr.mesh('sphere', 'neck_mesh')
neck:add_child(neck_mesh)
neck_mesh:set_material(green)
neck_mesh:scale(0.8, 0.5, 0.6)
neck_mesh:translate(0, 0.5, 0)

neck_joint = gr.joint('neck_joint', joint_angle, joint_angle)
neck_mesh:add_child(neck_joint)
neck_joint:add_child(head)


return rootnode
