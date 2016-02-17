local cjson = require('cjson')

if ( #arg < 2) then
  print("Please input jgs file path and number")
  return
end

local JGS_PATH = arg[1]
local JGS_NUMBER = tonumber(arg[2])


local allData = {}
for i=1,JGS_NUMBER do
  local filename = "0000000" .. i
  filename = filename:sub(-6)
  filename = JGS_PATH .. "/" .. filename .. ".jgs"

  print(">>>>>>>>>>>>>>> " .. filename)

  os.execute('rm -f /tmp/jgs.json')
  os.execute('nodejs jgsParser.js ' .. filename .. ' /tmp/jgs.json')

  local f = io.open('/tmp/jgs.json', "rb")

  if (f ~= nil) then
    local content = f:read("*all")
    local sample = cjson.decode(content);
    allData[#allData + 1] = sample;
    f:close()
  end
end

torch.save('./allData.t7', allData);

