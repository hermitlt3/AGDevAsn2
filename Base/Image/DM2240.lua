function SaveToLuaFile(outputString, overwrite, path)
   print("SaveToLuaFile...")
   local f;						-- The file
   if overwrite == 1 then		-- Wipe the contents with new data
      f = assert(io.open(path, "w"))
   elseif overwrite == 0 then	-- Append with new data
      f = assert(io.open(path, "a"))
   end
   -- Write to the file
   f:write(outputString)
   -- Close the file
   f:close()
   print("OK")
end

function RewriteToLuaVariable(outputString)
	local f;
	f:write(outputString);
end

-- CalculateDistanceSquare
function CalculateDistanceSquare(x1, y1, z1, x2, y2, z2)
	local distanceSquare = (x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1) + (z2 - z1)*(z2 - z1)
	print(distanceSquare)
	return distanceSquare
end

function TestSum(...)
	local total = 0;
	for i,v in ipairs(arg) do
		total = total + v
    end
	print(total)
end

title = "DM2240 - Week 14 LuaScripting"
width = 800
height = 600

-- Keyboard Inputs
moveForward 	= "L"
moveBackward 	= "S"
moveLeft		= "A"
moveRight		= "D"
reload			= "R"
jump			= 32

-- CPlayerInfo start position
CPlayerInfoStartPos = {400, 0, 0}
CPlayerInfoStartTarget = {0, 0, 0}
CPlayerInfoStartUp = {0, 1, 0}

-- Way points
Zombie1 = {x=-250, y=-3, z=-250}
Zombie2 = {x=-250, y=-3, z=250}
Zombie3 = {x=250, y=-3, z=250}
Zombie4 = {x=250, y=-3, z=-250}