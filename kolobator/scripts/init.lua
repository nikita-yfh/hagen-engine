Level={}
Global={}
function extend(parent)
	local child = {}
	setmetatable(child,{__index = parent})
	return child
end
Entity={}
