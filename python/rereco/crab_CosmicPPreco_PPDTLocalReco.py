import CRABClient
from CRABClient.UserUtilities import config

config = config()

# All output/log files go in directory workArea/requestName/
config.General.workArea = 'crab_projects'

config.General.requestName = 'crab_CosmicPPreco_PPDTLocalReco'

config.General.transferOutputs = True
config.General.transferLogs = False
config.General.instance = 'prod'

# Set pluginName = Analysis if you are reading a dataset, or to PrivateMC if not (so you are generating events)
config.JobType.pluginName = 'Analysis'
# CMSSW cfg file you wish to run
config.JobType.psetName = 'CosmicPPreco_RAW2DIGI_L1Reco_RECO.py'
# Increase virtual memory limit (sum needed by all threads) from default of 2000 MB.
config.JobType.maxMemoryMB = 3000
# Number of threads to use.
#config.JobType.numCores = 2
# To allow use of SL7 CMSSW versions that were SL6 at time of original DATA/MC production.
config.JobType.allowUndistributedCMSSW = True
config.JobType.outputFiles = ['output.root']

config.Data.inputDataset = '/Cosmics/Run2022G-v1/RAW'
#config.Data.lumiMask = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions18/13TeV/Legacy_2018/Cert_314472-325175_13TeV_Legacy2018_Collisions18_JSON.txt'
config.Data.inputDBS = 'global'

# Units of "totalUnits" and "unitsPerJob" (e.g. files, events, lumi sections)
config.Data.splitting = 'FileBased'
# Total number of these units to be processed.
#config.Data.totalUnits = NTOTAL
# Requested number in each subjob.
config.Data.unitsPerJob = 1
config.Data.outLFNDirBase = '/store/user/fernance/'
config.Data.publication = True
config.Data.outputDatasetTag = 'CosmicPPreco-PPDTLocalReco'


config.Site.storageSite = 'T2_US_UCSD'
