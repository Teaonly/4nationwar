require 'torch'
require 'nn'
require 'nngraph'
require 'optim'
require 'lfs'

local toy_data = require 'toy_data'
local model_utils = require 'model_utils'
local LSTM = require 'LSTM'

local INPUT_SIZE = 4
local OUTPUT_SIZE = 24
local CELL_SIZE = 128
local LAYER_NUMBER = 1
local BATCH_SIZE = 1
local MAX_TIMING_STEP = 64
local GRAD_CLIP = 5

toyRNN = {};
toyRNN.model = LSTM.lstm(INPUT_SIZE, OUTPUT_SIZE, CELL_SIZE, LAYER_NUMBER)
toyRNN.criterion = nn.ClassNLLCriterion()

-- global var for training
params, grad_params = model_utils.combine_all_parameters(toyRNN.model)

-- init weights with simple uniform random 
params:uniform(-0.08, 0.08)

toyRNN.clone_models = model_utils.clone_many_times(toyRNN.model, MAX_TIMING_STEP)
toyRNN.clone_criterions = model_utils.clone_many_times(toyRNN.criterion, MAX_TIMING_STEP)

-- init internal state
cell_data = torch.zeros(BATCH_SIZE,  CELL_SIZE)
hidden_data = torch.zeros(BATCH_SIZE, CELL_SIZE)

-- training function
local feval = function(x) 
  if ( x ~= params ) then
    params:copy(x)
  end
  grad_params:zero()

  ------------------ get minibatch -------------------
  local step_number = MAX_TIMING_STEP
  local x, y = toy_data.get_batch(BATCH_SIZE, step_number)
  
  ------------------- forward pass -------------------
  local rnn_state = {};
  local predictions = {}           -- softmax outputs
  local loss = 0

  rnn_state[0] = {cell_data, hidden_data}
  for t=1, step_number do
    toyRNN.clone_models[t]:training()

    local lst = toyRNN.clone_models[t]:forward{x[t], unpack(rnn_state[t-1])}
    rnn_state[t] = {}
    for i=1,#init_state do table.insert(rnn_state[t], lst[i]) end -- extract the state, without output
    predictions[t] = lst[#lst] -- last element is the prediction
    loss = loss + toyRNN.clone_criterions[t]:forward(predictions[t], y[t])
  end
  loss = loss / step_number

  ------------------ backward pass -------------------
  local drnn_state = {[step_number] = {}};
  -- there is no loss on state at last step 
  drnn_state[step_number][1] = torch.zeros(BATCH_SIZE,  CELL_SIZE)
  drnn_state[step_number][2] = torch.zeros(BATCH_SIZE,  CELL_SIZE)
  
  for t=step_number, 1, -1 do
    -- backprop through loss, and softmax/linear
    local doutput_t = toyRNN.clone_criterions[t]:backward(predictions[t], y[t])
    table.insert(drnn_state[t], doutput_t)
    local dlst = toyRNN.clone_models[t]:backward({x[t], unpack(rnn_state[t-1])}, drnn_state[t])
    drnn_state[t-1] = {}
    for k,v in pairs(dlst) do
      if k > 1 then -- k == 1 is gradient on x, which we dont need
        -- note we do k-1 because first item is dembeddings, and then follow the·
        -- derivatives of the state, starting at index 2. I know...
        drnn_state[t-1][k-1] = v
      end
    end
  end

  grad_params:clamp(-opt.grad_clip, opt.grad_clip)
  return loss, grad_params
end

local doTrain = function(num) 
  train_loss = {}
  local optim_state = {learningRate = 0.02, alpha = 0.95}
  
  for i = 1, num do
    local _, loss = optim.rmsprop(feval, params, optim_state)
    print('>>>Iterating ' .. i .. ' with loss = ' .. loss[1])
  end
end

doTrain(10);

