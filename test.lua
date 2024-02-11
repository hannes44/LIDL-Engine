package.path = '../../?.lua;' .. package.path
engineAPI = require("engineAPI")


GameObject = {
	xd = 2
	components = {}
}

Component = {
	name = "Component",
}




--test = vec3.new(0.1, 0, 0)
print("dfgdgfdg")
engineAPI.CreateGameObject();





function update()
	print("LUA: Update()");

	transform:shiftPosition(test)
end

function initialize()
	print("LUA: Initialize()");
end
