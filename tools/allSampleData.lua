local getSample = function(game, who, step)
    local sample = {};
    
    if ( who == 'E' ) then
     
    end

end

local allSampleData = {}
allSampleData.init = function(file) 
    local allGames = torch.load(file)
    
    local splitNumber = math.floor(#allGames * 0.9) 

    allSampleData.trainGames = {}
    allSampleData.testGames = {}
    for i = 1, splitNumber do
        allSampleData.trainGames[#allSampleData.trainGames + 1] = allGames[i] 
    end

    for i = splitNumber + 1, #allGames do
        allSampleData.testGames[#allSampleData.testGames + 1] = allGames[i]
    end
end

allSampleData.getTrainSample = function(i, who, step) 
    return getSample( allSampleData.trainGames[i], who, step)
end

allSampleData.getTestSample = function(i, who, step)
    return getSample( allSampleData.testGames[i], who, step)
end

-- return allSampleData
allSampleData.init(arg[1])

