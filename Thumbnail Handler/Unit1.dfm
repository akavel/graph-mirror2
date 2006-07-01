object DataModule1: TDataModule1
  OldCreateOrder = False
  OnCreate = DataModuleCreate
  Left = 337
  Top = 167
  Height = 150
  Width = 215
  object EasyFactoryManager1: TEasyFactoryManager
    Left = 48
    Top = 8
  end
  object EasyThumbnailHandler1: TEasyThumbnailHandler
    Registration.CLSID = '{1944F5A1-2835-45B0-91E6-FA3EDDAF539E}'
    Registration.Description = 'Graph Shell Extension'
    Registration.FileClass = '.grf'
    Registration.RegType = []
    OnRegisterOverwriteHandler = EasyThumbnailHandler1RegisterOverwriteHandler
    Initialize = EasyThumbnailHandler1Initialize
    ExtractImage = EasyThumbnailHandler1ExtractImage
    Left = 120
    Top = 64
  end
end
