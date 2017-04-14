object Form10: TForm10
  Left = 371
  Top = 262
  BorderIcons = [biSystemMenu]
  BorderStyle = bsDialog
  Caption = 'Tip of the day'
  ClientHeight = 210
  ClientWidth = 422
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  ShowHint = True
  OnClose = FormClose
  OnShow = FormShow
  DesignSize = (
    422
    210)
  PixelsPerInch = 96
  TextHeight = 13
  object Button1: TButton
    Left = 338
    Top = 16
    Width = 75
    Height = 25
    Anchors = [akLeft, akTop, akRight]
    Cancel = True
    Caption = 'Close'
    Default = True
    ModalResult = 1
    TabOrder = 0
  end
  object Button2: TButton
    Left = 338
    Top = 56
    Width = 75
    Height = 25
    Anchors = [akLeft, akTop, akRight]
    Caption = 'Next tip'
    TabOrder = 1
    OnClick = Button2Click
  end
  object Button3: TButton
    Left = 338
    Top = 96
    Width = 75
    Height = 25
    Anchors = [akLeft, akTop, akRight]
    Caption = 'Previous tip'
    TabOrder = 2
    OnClick = Button3Click
  end
  object CheckBox1: TCheckBox
    Left = 8
    Top = 186
    Width = 289
    Height = 17
    Anchors = [akLeft, akBottom]
    Caption = 'Show tips at startup'
    TabOrder = 3
  end
  object Panel1: TPanel
    Left = 8
    Top = 8
    Width = 315
    Height = 171
    Anchors = [akLeft, akTop, akBottom]
    BevelInner = bvLowered
    BevelOuter = bvNone
    Color = clWhite
    ParentBackground = False
    TabOrder = 4
    DesignSize = (
      315
      171)
    object Label1: TLabel
      Left = 64
      Top = 24
      Width = 104
      Height = 16
      Caption = 'Did you know...'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'Microsoft Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Image1: TImage
      Left = 16
      Top = 12
      Width = 32
      Height = 32
      Picture.Data = {
        0954506E67496D61676589504E470D0A1A0A0000000D49484452000000800000
        00800806000000C33E61CB00000009704859730000375D0000375D011980465D
        0000000774494D4507D70C1F0D2130ACFB9AE300000006624B474400FF00FF00
        FFA0BDA793000038BD4944415478DAED7D07801CD59176759AB0B3BBDA5D8555
        5AAD222820826CC00221301C1901876D7EECBBF3D938FE67FBCEF6E11CEFB7F9
        7F731803779CC3D9207CF6E13BCE186C6C134C0E320A801028E7BCAB4DDA3479
        BAFFAA7AEFF5BCEEE959090CDAC56CDB4DCF8C667A7BE6FBAAEAAB7AF55E1B30
        BABDA53763B82F60741BDE6D94006FF16D94006FF16D94006FF1ED4D4300EF01
        30F1300DF7C9B8A7BC9259EB15702F190E58F680917006CC586C004CFB30D4CE
        D90D73BF3888EF738DBA4BDDE1BEF691BC8D480220D8293C2C2EF6C1D2521A4E
        4218E78067CC748B46DC2B1AE2B23DE48361C86F20BF8687477AD9363CC3B20E
        81E5EC346C679B5953BBD29E3E7F8531E7633BF15D19DC73480C6FB8BFE748D8
        460C0110F4F9A57EB8A6D80B17B80558E4660CC773F1F2EC1818C90960D43483
        91180B106F0023560F86190704587C05FC80E7E601F2FDE015FAC0CB7623CC1D
        E0E10EA53C789E87EF476E2462078D446A9539BEE5A1D8DB3FF010245B7AF104
        FD4886FC707FFFE1DA86950008FA94D200FC55A107DE87C793D0DAC170126034
        CC0273CC4C30EA6780513B0581461258362288BB6101A3C9D66F00FD8F00C6B3
        E1FF71774B782C2121F058426264DAC0EBDF035EDF4EDC770014D3E28BC7ED3E
        ABBEE10F76CBFCDFDA8BFEEE293C2791A10FC950186E508EE5362C0440E01714
        BAE0BA4227BCAFD00B31C38E83D9381B8CB1F311F859004E12E835B01DC43B86
        C0A3A523010C2280694A128850400781BF2B1ED0D12D22078808457E0C257CCE
        4724C4E03E707B368077782318C5413E8F5993D8624D9ABD3C76E6A71F00BBB6
        0BCFD68944C80E3738C7623BA60440E017E5DBE09BF94EB80CDDBD61D4348239
        EE04DCE7A35BAF038811F04924007A0174FDC2F2631AF896D8C90380F20064FB
        D203B8AEE60588049200448862411CF97949848DC39BC1EB5C035E7A1F9E0ACF
        8521C21CDF7A677CC93FFE1262F53D78D27624427AB8417A23B7634200047E5C
        F1307C3BBB0F3E82C2CE34930D60369F82563F075D31EABD580DBAFE1A76FF02
        7CB2FEB80F3E1380AC9E8FA62400082FE04B3969FD72A710A0C240D90BE0E362
        419240ED489F815DE0B63D0590DE2B78158FEFB4A7BFFD06E7D4FFBD124F8C82
        020EFEB9868637940094BAB959F8786E3F7C2BD7064D64D566F389608E5F883F
        722D5A7D4AEC71B27A4100069F2C9F3D80E35BBEA12C3F10FFE5DF51FF759516
        28692410E08BB020C20087027EAD208841EFA7F71EDE00EE814790247DFCCB98
        B50D8F3A27BEFB26ABF5ECFD78E236DC3BFEDCB287378C00087E33C6F8FFC8EC
        40558FBFB5593719ACA98BC1488D47D06BD9F205F83522E613F88E049FD4BD06
        3EC57CB67CB30C7EE5A54B5C941770CB9E001411DC32113CCC0EA08210B81733
        488287C1EB7949FC09D3ECB7262FBC3E76E667FF80CFA8B6B00B49901B6EE05E
        AFED0D2180F73B383FB31BFE2377002682698035F164B0269C84E95C7DD9F295
        EB8F2504F8E4EE6D09BE6EF5B483041F34E52F2F9D62B7C80220A807B470C064
        F04950922428F00EDA512782D7F30AB8FB7F878FF3E2CFA7C6FD2AB6E4BAEF19
        751307F00FEC4112F40C3778AFC7F6BA1200AD1E3095FB46661B7CBDD88FD03B
        36582D4B31D6CF42F0EBC044F0219E92D62FAC9E63BD74F7A64AF52C25F404F0
        A8D3C123E095F803E505F4AF6030F8FCCC53FF410FE06707254984A2F40A5213
        940419481B70CD402745FA209476FD37FEDB009FCF88C5B7D9F3DF739D3DFB3C
        140BD081FBBE377B4878DD0880E05B183A7F90DE021F71732CA4C06E3D1773FA
        696026EA05E80909BE2FF628D60B8B17966F0ACBF7637DD9EAC5513D57976E94
        85201FAB87054FF70832E6FBE14011A188D65E1424309804F838D309EECEBBB8
        C0C42430AD4E7BCE459FB64F78D7267CE130EE3BDFCCE5E6D78500087EB2D803
        BF18DC0257D06F6BC613604D3B072D7F3A5BBEA1AC3E5E232C9F8077A4C8B3A4
        BBC738EF21C04648E80580D69F073C81AE09D4634F3B02F8852245004D1FF8E9
        21015E941E402383973904EE8E9FE1EB19710ED318403DF305E7ED1F5A8527EE
        C77D3B92A038DC60BE96ED4F2600825F8379FD0399ADB0947F1B72F593168135
        6E1E3F16565F23C14F88386FCBCA9E252A7B865EDC09BB792304AE111502A2C8
        A05E032D24281278016FA04242591BE445BA284940DEC1EDDF01DEEEFF16EF05
        36F8BCD57CCA579CC59F7C021F53AD60EB9B3155FC930880E0DB68F9F70E6E82
        CBE8E735538D608E9902D69433CAE027A4D5C794CBD7737B057CC8E28D21AC3B
        FC5AD5F798A1F7A88BA6FF485D002A537065D5B054D604AE240011A290E314D3
        3D88D941E76A7EBF419F23124C3EFD1F9DD33EF21C9E8C840291A034DCA0BE9A
        ED351300C13750E8DD3EB8013E48BFA3911C83A9DE58B0269F0666FD44CDF205
        F826016F6B255DC394299E04CBB77E1DC428600182560EE5D78DA1C2438447F0
        BD80AA1DE8E140A58A7A3840126CF917F0F283B2DA489EC0489BD3DEF909E794
        F7BD8C4F683C61FB9B4918BE660214EF81EB11FC2F7396944C81D984F13E1E03
        7BEA1261F555C057F1DE07DF17760A7C3328F2A280D72DBA22FEC3D04430425F
        394000978B42864602951930095C03DCCEE7C03BF8283E976183496076D80BDF
        7FAD3573C901109543AA150C37B647B5BD2602609EBF6CE015F835664786896A
        DF9C301F8CC2014CF950F88D992A0940E0C759EC197234CFABA8E88554BE0FBE
        56EAAD4A04ED38E4EB436905FE365A58507583A2D40605BF58E42941E8E1F7D8
        F8CFE27991FE4D7A0223B6DE59FAD58F9B0D93A958B41F097068B8C13D9AED55
        13005DFFD4CC2E589BDB0F630DC7046BFCF134B40A66A91DAC39D7F8E09B047E
        4C8EE89932BF37556E6F4130ADD3810993A29A108C00DC7FA81A44D4FB0122BD
        81FF81501551A5899E1C34929E8088E05A8DE06EFF216AFF3D9C3178054902C0
        DD69FA65FC921B6FC0272406B7BE1906925E150128D72F74C1A328FACE662FDE
        3815CC06B4F8FC2E306BC780D57A191240800F8E26F84C3998632AAB372B73FB
        C06B3AF8557441204C1CE5D7F4078F42A40B0C284971E88635814C0FED260C01
        BF07B76305C28C5EA050F03D8187EAD0683AF1F3F1A5FFF038943383112D0A5F
        15014AF7C2E7FB5F861B806AFBA97A30C7CE446BC7DC3DB715ACC65630A75DC2
        566F3A820022CDB3CB6EBFC2F2C3E04790404F0BA3ACDF88F002FEE6553E0CEB
        8BA148A011C0F06476603783D7F108B8ED8F23F8D485A4910049830942A7B3E8
        EFDE6BB72EEAC493742101F60E37C8436D474D00EADE496F834DF943506BC6D0
        E58F9B8956DF883F4C1798DE2154FEE3C06CBDDA8FFBBEE5FB31DF0A01AF2B7F
        A3CA73F57E2D5B8874E36194B59C1FBC88F7448410754E3F43509EA02805A210
        856E6206C081FF06AF7B0D660339DE8108A09100EC31F7C52FFBDEB7F004541C
        A2AC6070B881FE930950F82FF8CF81F5F03EF6E4F5E3C16AC054AFB6018CCC7A
        74F945EAB70363FA5F83191F233B792CB67EBF8307B40A1F844381192489213F
        0B96F69A299F47C5701D7C17FC14CFCFF74B65407DD1075AC15096919537F139
        A38B42DCAD5AF46C93C0DD712300B59A1101280CE82460BD80F1A0F98C8FC6CF
        F8F00B20EA032336353C2A02A0F52F45D5FF4409553FA57A56D3541A2B47D01D
        30731B004804C651E18F3B158CC65365AE2FC13335F021ACFE35B0D54E6211B4
        FE3F087B8E2811A8A3091A01647AA7DCB9BE43D83BA87399FA370F8E2A266781
        97DE851A7FB9A80B14B25C24F2380CE4E551640EAE1B7B2171C94D1FC3749886
        8E292BE81E6EB05F33017277C2D3991DB084F141EB37EB9BC0AC6B446C336014
        510022018CB8238675272E03888D15757D458200F0EAB102D9113D7F86DA1519
        34A244E6F3510408F3411B1A8652D9957B458D0C2E840402043D8CF41CF116BC
        CE1A807D3F062F87E93E815FCC3109840ED08850E09E44CFA85FF0D9F8B99F25
        4148FD855B47E2A0D1110980D67F065AFFB3A5416AAB468137A65928FEFAB160
        B807F0049D9C0672BE6F237031D40563CF4323AE0DBA6F9F0896049E9A3DE5AE
        C067EB0F7908DEF4BA0000541583FAD7F1B483B276B74C02CAF17532F8BA214C
        2E53806F370074DE8F0E7D3D7209F12C21E0C5AC2000EEAED2037949004A114B
        F6CBF18BBFFB412351ABBCC088EB21382201F23F87FB50FC5DC1B8D591E537B0
        F55B754880FC06C43387D8393CAC2B86767177EA011ADE81C746CDEA9526A0F7
        C425F0EAA8B57B4755EBA25CB51121E0029EC20CBEDF3F8FA77982428808216F
        6026001233C53576FF0113BBCD222370A92884C01769CFC950807B2E2B85A11F
        0A3CAF61D1C713677FF28F2026A48C382D302401D0FAE7A5B7C02BC55E30C9C5
        93D2B76AEB2511C68131B85A1AAF04DFEFE6B105C8A9B918376768E0C7B55D11
        40593D84841A55D7428ADD179100915EC17F9F7A1E169C92607E9AA73C81DC49
        EC51A8A0EB8D4DC67D125AC02180C38FA195F7809F1550FD9B0A436E96BD0093
        209F157B2E2308A0D24337FE54E28ADBFE1EAF938A437B9100FDC30DFA5113A0
        F83F70E3E046B88E7418E5FD562D0DF88C1104488D4577F82C1A3402E9D8A297
        CF075F13750EBACE9AE3D18D4E922E5F27800D658B94D5341F5405788478AC5A
        1A0ED0177CC5EF13427922198EB891547A022F2FAEC76AC26B1E8F97D207D0BF
        06ED764BF9FC2AA3E05143410070A50E600D90C1A719190AE839670639A3F5AA
        AB62A75CB61BC4C493115517A84A00EAE8CDED87DDB936984A255F1EE94BD572
        08306A9BC0AAC1183FB0AA6CFD72F2862FE45800AAD76BC48F9A44771A9F2808
        C0EE56BA5F8552608C20346610B6E2C850015ACC87B208D4EB02BE5791E1C848
        8AEBB3EA842EC9213E83AFA06CDB2A3F0F10209D4A2FDD822C0FA3D5A31EA070
        4004505EC0CDE5CA452273DCBF262EFDCEBF0B96C18E91D43C321401CE1EDC04
        4F88F6AE045A3FC6FE549DD000B563F93563708D367BC7F62B7F01856F611CB5
        5272AF11E0533E4DBB5D474389E207F5AD5FB3D270065151411CC2F2FD620E94
        633B9F332EFEA69992A0E3B90A9D08561B82BF1381EC95969D17D7E4E93F9551
        3E9F1C272031C85EA098116230475941461C91042E9120079BE317DE788D91AA
        271D40934D0E0F37F0472440E16EF821A67E1FE30A2E957D6B10FCDA3AB47E4C
        FF524DA2977F6035E2AC5BBFB22AED315B97023F21C5564E56FD6CE91D502CDA
        E87AED7A916A11697CB117BEE2701F40B842A87B105953604F1493D5BC7EE1DE
        0B5D0836025FEA15EFE17F2F49F0856B67807D42C96291AA37A802114F4C1559
        0101EF15324207E4A428CCB11728C2F8F3DE155F7C0DC5930124C0BEE106FE88
        04C82D87DDF90E9866D898CFA7D0EA6B522C000DEAFA210F40ED5D14024C65FD
        8E66F98A00718D004971622FAB59B9137C7FC073D4484F9114E7F1330647F30E
        56199080C257CA9EE27446EE69F15CCFFF9525D3E7E8DC44042FAF91800850AA
        0C057A4309BD4765059C0D647C0FE06588083931746C8CFB6EE292FF7B87B838
        6E241D118344910440F73F3DBB1B7652EE6FC450DCD59007C0F8AF0850831E80
        7AFAFB570663BF6907494016CF40D60820F9072D68A08708604678113D1C04CA
        C2BA0E08C77C572B056B47BF20E46A2450E3F9A6B85E72FDE4A15418F02B8911
        3F9B6A24E191C26C9904F9347E1CC1CF8A704062D0CD1B2B13CB6EBB16F3298A
        2D0747CAF84034017E0FEFC7F4EFA7AC991235027CDC0D0E058D48080A012806
        7B9F19C2FAA5DB550450B15EA9ED0A124410C87F1C220044D40B0282CFD5800B
        93418DF5BBC1F7295148A0BBB9900E50A969282DE53FA73A8B73520C66443680
        04805C9AC3006504A55C61C09CFEDE8B630BCFA12966DD23A5341C4980E2DD70
        7BEE005CCBE13489A0276B7C02F08CDE04A676712441FF2AF1E3999AD5068075
        8204A0E7EC015DAD22E8689FADE211F442912F0C238A4501A0BDA0C5FBC3BBFA
        D8807C9DCFE788CF10F85E4E92201F24923F9014221EFD1B37928ABA006703E4
        01F29200B9AC108376EBDF272EF8E203F8A93412E0C070835F9500F99FC30B85
        6E38C5B04C319D2B99945E00451ACDEC4DA01788378A7489626B550B8EC92C20
        2908408FF9875639BF59268FE954F702FED8800DC151441D7C5544D2D609D0C1
        570D1E01ABF7E4B96CF9998246807C70CC2092045A5650525E40EA004E05D32C
        08212B4BC585D8DD8965B77C455AC19E915015AC20004DEFC2ACA80FB3A13AB0
        2DD1ED8B04308800DCEADDC03B20018C0CA54D5DD1F1DB2745AC0C3E13419280
        FFB22B2EC19FF7AF79037E6C41A4168874FF4788F715C0D3E7A55751C291DD7F
        5E8A455D309682A144722EF0F3A93507280C14441800D20299B49F11B8B9D20E
        67C93796598DCDAA6F70D8EB01510498946F8703B45C0B380E62572FDABC9229
        49867A8EFF34E803C51E64F78EE8F42F4082B820802909602955AF2C5A5E0981
        EAC7FC88EC00C22250221129F874D09577507F487DDE0D8D0948E07DD7AF835F
        0CF516C873797A6DA02886898B42088A503028F400F70CE40BEE98732E4C2CBE
        1A7F34AE070CFB2A245104588AF1FF49FA4DB8BB2751A71180C0570468103FCE
        E0CB95E22F8A103E0912B2142C89C0299E536E18D5278898BAF80BF707A80B8E
        005F0F05515FD5B76A2D5DF4C703C83B1783D6AF6B868AAC406B22612F9097C3
        C44204BA59F20069910E1672E07ACD9F4D5EF8B55F8168171B188904786F762F
        DCC5FF1843E01398C327C4B14C00EAFAA11DFFAD6F65654E5FE101344FA0769F
        0C6A7C20164AFFC25D42A112316F7A5C9696EE83AEB78485C61B94D5FB477D0F
        83AE03AF87023D2B903FA3EBCA3090976120CD6303904D6B8344F65DF14B6EFE
        12BEB11709D03BE208E0FE0E3E9A3F003FE27F7150FD276B45D997A779D50A02
        C4EAC5200F0DFB0EAE17932623C18F48F37CC0350FE0F70568FD011522507517
        291D1072FDE154CF7F5E0C01AF9EABD13FAD37C0FF9CDE2CA2D70BC21984E29A
        2200C86A63410E11A74528C892279035826C719373E63F5D6AD637758D4C02FC
        063E8B1AE026F6C654EC49840810AB1304200F40042874A0ABDB17B4F80AEB0F
        132326C10F8D0EF27BD4D1099ECFF70A6A240FAA001E065107BF2400F78A4132
        30F8A1DA4194800C741185FA0B3DCD0BF07A03E8EE8BCAFDA7B928440345A803
        72A5D4594B938BDFB365248C095410A0741F7C0DB380FFC3429B56F140E069BA
        B79950AB7A20019C3A049F0850274E31B0AE0AE8D50A3D312DF5D33A834C0DFC
        80177034EB578520BDDD2B4C8470FC2E56122160F161F05D888EFDA1EAA1AF33
        940E90BD06FE0C635915640264649510C3803BEE23F1F3BEF2CB9149805FC1B7
        0BDDF015E101D4A20E0939E32725967373D01390F5D3681E0DE0F4AF153FCE11
        2B7C4ED90398FABFEB7500FD1851085242515B05A4ACCE75F0AA798252F0BD91
        A0477993081DE0EB0EEDA764C75092EB0CE8D9405AF60DD26EFE38FE17377DC1
        A8BB68D85BC42A09702F7CB9D005D7B3A7D54300CDF56302D472BF9F41C09307
        2012140E6118383084CBD735800670800055C607FC0C20DC4A0E50D14114150E
        20FC5A14F0E1D7A2424A151DE0B791C994502E5A29DAC373AC03445128238561
        9ECAC22FC6167DF52FAC96BF19F67270A506F835FC7DBE036E6502D0EA5D54FC
        A119BE980988459D52C20310F044001AB12300C90B9811000674816EEDA19471
        A8FA7F64FE0F008171FFD07C8048603D387A1284C713425AA0427C06C380586C
        2AEF8F0E8A2C40F60E160B693733EF84E455BFDA39F208F05BB81645E0ED9C86
        631A08540788C7A507A0859DA417E04912B5A2A982C890D98A8C1FAC32983304
        01021EC30A597DC4AC22BD4F400F03E172700519F452B146862392400F01A10C
        C12782DE33007E1810B38A25E89204A29D1C8931605F11BFEAA5DF8C3802780F
        C0BB73FBE17F5804D2AA1E8A003CE9537A014776D3D0D1965E80C6DC33DB8F92
        007AA14827810E7CB82B48357B04D087A017087B8228CFA07B810812541B4A86
        082F10280F6B5A40E900353E201B456850C89F4C9229FEBFD8651BBE6218C6B0
        8E074411E06DB983B086BF841303333986177EE029DFD4054424202FC01E2025
        5BBBE471708328A6448EE885C18F1AF8B182EEDF6FE20C4D1C0D5E71E5AE8784
        484254792D2A4C546882B017D0CBCDFA657872C16A9A40AA798182EC22CE649E
        2CD6FDDD79354BFE61581B43A208508F1AA0977B012D8B1B40A8298434008B41
        F6004931F1C396BD7E8A00A50180ECEEEA43BA152120820015CD1FFAE04FB815
        BC9A1708798300D84391A04A3349D400534563895E18923B2F435B141DC37298
        5884006E14E94D6F70A68FF9D4BA614D05238783733F8336370DCD8665F0F8BF
        9148B017301372A52F5AD7D707BF4623414A540679BC5F07DE8AF00011E3FDA6
        AEF8C353CA86E8048EF202553D42981CAFC61B54291405C4A0BC24577610F35A
        433939812423456096338252AF7B6EE2F20D8F8F3802E47F0E4F15FBE12C2200
        F50050FC3712BA10A4B5FE64A3877FAC1504A061C4ECAEE85E3F33AA3610167D
        61E1179E362E2FDBBF722FE218067F280284BC8257C51B54A8FE88C2903E1D9D
        4FE1C985230AC2F28908940D94449918B2A51BECF35EF9D270EA804802147E01
        37167AE03AEED7A042107503D1BC40AE07247890086C24015B7E526BFC947B66
        9B10856604012A6A0496E625F4B6AF50EA670CE5018620C051ED6EE5631DF840
        C5B15A6A181A24525541D795CBCF8A514290EDE31E194AA1F862A1E3ECD39257
        DD366C7D0191BFA67B3F2CCBEE81DF884C005D7F0A8560521000A82C1C933775
        F03D4032D8FE4D3F427A8B066EB8126847788088C64F3FFE5799161EB8FA3001
        AA91813E57C50B78114438EAAC401DB5BFC3DA5016853C313E407A806E5BC35E
        A05474DD1E6766ECE255BB471401500836E6F641270A4193660519A926CC0662
        E574309E2813C04A96BD0077FE4812D0448B62B716DFC3EEDFD2727F7D2D014B
        EB0D885A492C626AB811264014213CED7DAFC61354AB1784C705C25E409D464E
        3FE335037260F0FC81B4B8C9152D3F93F73E662D5EFDEF4655CF360C04A02D7B
        07BC581A8493191B9A17904CF0F4702108D54ADF44024900063FA93581268417
        A0D13763280F5045F947C6FF500808F7050E4986A108205F37F4385E4D1B0C35
        5AE8CAA653AD4D9DF9E0CA5944AA652C2DEF6646D3C88BBF37C6DEB2CC9AB944
        4B2346000150087EA3D005DF642C92B22FA006C30011209690D9001140F6FAF9
        47025F7A064A81F4E2D051BB7F6DF9D87097500509F4074390C0880A0BA08583
        2378858A2567A2068C425EC08F22B4907D91571533BC3C1A7E96A798F312F55E
        31EF0D26A75B4B9E3838A208E0FD0E8ECBEC84CD3C37C0B17841283319E730C0
        24884B0F60CBCE1E3B51F602BA37A0E9D5D4381A09BE1D74FF51E95FE4EA61F2
        D275F0C3423A408208025490440F155A0661E8238E11A1205C1D74F551422D23
        E06EA19C10829E049F179EA4F0009F364F79F6D6E1080343FEC5EC4F60358681
        B7334E1C0692520CC6653620EFF1A3BA7B54D7AFA511819E677782980F58BE1B
        48B0F2674678006D9E5F60C938085604F5D2B051EDDB79431322101AF4C711A9
        629410D43B86C2BD027E51885E13B7A4F10B44FE6D6B8AAB4AD90F9EE12CFEE8
        31AF0A0E4980FC5DF01934E0EF71432E893F9A201A20405C8601D9E367493298
        E1B90008686687F8D175D73F6406101AFEAD584412CADE20E00934A0239710F2
        86208607D1A16288ACA05AC790D202AECA083C11125D755712FA77EA1DE49634
        CF2B3827586F7F62C388220065031806F6E075D7F21A01B54DA21E40A180E60C
        C62409E8B62FDCE8A9BC412244049AED5B9205223DEF0F157FF415C5C2F70F08
        0F06194722C110C0078E11BA21A00B0086CE08221A4FDC88BA80EB9597942DCA
        E5E4A0E08702D7F5FED53AE9E97F38D645A123069DEC9DF0BD521F7C86B14AD4
        8A8521381B4022C462C20338A2C7AF4C827890048A08EE2092FE60A5F59B5102
        505F4AAE4A31287CD4C1AF76ACFA6F472242152D101902C2821064AFA0FC3CDF
        BFB02884A15C88DA708B7DD9DED4CC9AA50F778D2802B8BF8596EC1ED88EDFC3
        1162B0498E0B88F101B132A8BCF193216EFB56EEF64D042785D0911660A046D2
        800E086B80B0FB8F5A42365413081C7570233286231EAB650E474807031E4027
        81CC22F8ED1EC77FC3D70172A511BE4389F75573D133D71F4B3178547F29B71C
        EE4431F8B75C1AA6B1019A24C22410A180430093C00992C0D0E70124CBE181B2
        029A553494F5479140015F017E48101E890CFAA872D57F0BEB82D0C052D51E01
        B50C9DAB1D15090CA105E86E660838AF3FCCF72A942B8DBAA52E2F939C639FF1
        C831EB153C2A0294EE85967C3B6CC4EF913268BE606AAC2480F40031B93238DF
        EE55CD02D29683F3E7016853C3C80B903730AB94800D03228560A42780E86314
        B88A18FABFE9640867150132846B02511AA058F6006ED80B1872251CAF7C8F42
        990DD01A02E23635EE77ACB73DFBA563E5058EFAAFE47F065F29F6C1B7190B1E
        20AA475C1DD601541B282F0F1F13AB86A8BB7E57904089C4B8589BA774B84A11
        48BF7B58D8FA4D0DB82AE12012F42AE40813C37F1ED605A174305203142BBD80
        E7961D484995168A817490BB88C92B944A5937639C6C9FF9CCE6114500CC08E2
        B9FDB01E493A8B8789D90BC4E512B1729CC02EDFF3574CFE50B383B50920814C
        212EC60B78CC208A000644168474EB0FDF2F280CA80EFE5064308620509800BC
        A24804015C8D00AE36BFD0D5C607A4B3E0077C236B716712A3545E67D82B951E
        C90D9E7E51EAEC5BDEF0BAC0ABF233855FC04585C3F07BFC94C17704A1661116
        828E582ECE0F05C2FAFDF583741228E02D4D1FD0C24DD45A3E9407087B810A12
        84C18F2240D803182102542387F66B552C3EA1E7FEC5501890D6AF6E6B2F0B43
        C20B88412215063813288AECC0E52CC1FDB475EA736F7875F0559F3DF753B8B9
        94814F33263C73B84E788198582F90534345004B9B00E2CF10D6570B55CF635C
        2685429BBCA22A04082C203914F03A8011FFE6836CC82EB3887F8B240494C10F
        6400C5213C805B1682AA28C43501109F2FAA5BD916C41D488A3233F08A192FEB
        9D6A9FB96AFD882200868258EE00ACC0EFF936D132D6241A4468C978C7116B0A
        6048F0E45DC08D00014273022D2D75A4904157933F207E44339C068641AF120E
        C2E04304A8CCB188CFF14B6604097422E91A204C802AEE5F2701471149006A1C
        75654AE8CAB5050AB2386438280CFB57153AEBCF4A5EFA587EC41080B6C2FFC0
        ECE2617801C54C1DD8087CCD388098F202B62481236F051F6A06E57583746118
        9A1B48B581628768300D805FA52650950411161D09B831F4AEDFB25E8503FD3E
        83DAFD062BC1D7CAC2ECF24BDAF8802281F402F4A420C52091A0662178FDEBC0
        CD0E7CDD39F3856FBD51A1E0359F15B3824B4A59F8353EB4A906404BC701EB00
        9B49C004F01790768244B06295C0072687C6444B5991EEBCE655825F41026DBA
        98710462E8964E9BF204FED1AC4E00531E03042846B87E574B05DD4A4FA00461
        49E80279971139AB587A819AB7E3EFD7086EDB3D59A3AF74A27DD1BAAD238A00
        B4E5FE03FED6CBC1721285345FC04C36F2C2D14400838F8E5C485A8602BE5DAC
        CC0E7C7DA01FD5DA00F2395D1EADE64925E428021C4D3818920C3A6122800EBC
        A6FF8D213C4085E547914066134A0BB00814E7E01B5111016ACF00A3F91A286D
        FC2CC040DB72FBACD5D7723BFE48228024C1E79004FFACA6931B89318204B4D3
        BC024778010E0DBE275024D00860E864D02790D2DA7D5941049E811C06DC3C4A
        B0ABBCA79AE52BD0214C08F5CD8728FFBA9A3E703542F0BF692292785094F72E
        2E4931C8F71EC28CA0F13224C05F417EE7ED90DE75CF0163D58E598DDFE87DDD
        D7147A5D024BF64EB81E0AF06541021A2CAA07AE18B217B084E53BE530E0DF5B
        C05077178B55EA84C0F472D93B48E9A24BF328BC2313E048FF36E46E6AA06BC0
        FB0490E5E18A1B4DBAD104081CB5E651061EA4009424400DE0E690002D5F02B3
        E92C48AFFF67B8FD8E87BDEDEDB12BFEE5BF9EBC7F44128049B01C3E8F9EEC3B
        F87B19347B08E263B89388979AB3EDF26E8990C0B791B742A0FBAB8EEA2450CD
        A36AA53004C1ED178B3C1B5E04D0E610561EE1194CEDF3FAFBA2C8C0BF9821C1
        D74A7BDABD878320BB95C470B5F717C591A7901545F7702153807C6932A4167E
        171D5F07FCFEFB1F84E5F7EC82F53D89ECC429D3AE7CFAE9671F1A91046012DC
        011F4232FF007F23873B85120D881991C066321896080DFCDC8AC9D080E90E6B
        0347035F6B208D9C46268B45A40D880C24C0C2165CD5DD9B32F71F0274FF7938
        0330B45F4C1B1CD2EF2CA6EF437802AFE482C15540029F169470213350A085C5
        A0F6B8CF83553B1B36FDFE0B70EB8F56C1E6BDDDB0B6CD856432999D3163C65F
        3EF7DCAA0747240198043F81B3F03BFF121F4E60574F9EC056C05B2234D03D06
        94382422D8B67FBB19C3D2DC7E6071A9A84E22B5745C4E84075E891CAA93C007
        3714DBA3AC3E3245943F991A23F0B441A230115CFD7129F8587A0276FD8522B8
        051706FA4B50CA1621D1BC0CE2939641EF4BB7C2438F1E84752F6F83FB5EEC80
        F6EE7ECC168B108FC79904AB573FFFBA90E00D492E91042D6E81979A5B62D08F
        1DAF15B79493042022A8A291A1C0B7D49D47141904D046D4E2117E5FA13EA780
        C20381405E8196DF2B5527810A1BD508A0BC4424F8E16FAB8703AF8A17D0B442
        A90CBE818016722518E8A3D5C44BE0246743CDCC4F406EF7BDD0D36EC0EE3D87
        E1B18D3DF0C4DA5DB07BF72EE8EFEF877C3E8FF66267A74F9FFE976BD6BCF027
        93E08DA92EE096F977B0D0337F097F976FE05FB19900B4BA88EF052C2E1C8112
        87EAAE63965DAE1958B6468410017C4F209B4A4D2B480A6A4225327899D74080
        6AE9A24E02F5D369DDBF9E3B744828290FC0F301209F2BC2605F8987824D4842
        6AE6A7A0D8BD1EBCBE3C4A8246E82CD5C10B6D16BCF0C21AD8BC7933ECDFBF1F
        BABBBBA1B7B71779E566E7CC9973168683352392006A1BBC154EC6C30FF01BBE
        83169FE699C5DC3C62CAD0E0946B07B6ED178F846710C07BA625C5236905410A
        CFBF23799924625651A8C790D3F6B4D00BE4157421181680815272380C40F973
        153F5B98005AB9582F00B9AE3F0C9CCB14D1ED1739ED330A25484E79AF5846AE
        A7072F7B0A78A89F0AADE742FBA10ED8B66D2B6CDFBE9DBDC0A64D9BF0F976E8
        E83804679DB5F4B4FBEEFBF5EA114D00DA0E5F8FF0D4C087F1E13FE1EF3A91C0
        F5980836FE8314854A1CFA44909EC096D66FDB52235865C0FDFB1459C15BD506
        BA8D1511C8A717A557905A219C159843587F54138A3E19450F03FE809117005F
        D4FD4B904917213D50E48A9F89BB33E634B06253C1EB3C80DC9FC9D71A3BF16A
        70AD38E47239B6F84387DA99046BD6AC86279F7C1276ECD809A954CD0FDFF39E
        AB3F83DEA0FEC61BBF7B68C412406DBDDF865A33019FC23F4AB7A26B12623021
        DCBFAC1988B10402563EA6D7748F6059E5D0A09EEB5EC0146460AF116E3A555D
        C70C74567806EACCD5C59F1A211CAAD218A5052A08A0C0F7FCC610AAF665D2B8
        0F8A615F035DBF654F00A7E11D503CB41B9CC47CF40619704EB80ACCBA89F2B4
        1E3A89020C0C0CA0E56F83071F7C001E7EF821F406BB5913D018011200264E9C
        78CBD6ADDB3F33A209E013E11B506F24E08388C9A7108C596CF5DC5EEEC8C291
        563BE0F4D19299825D26852948223C842485255F37C53974B1A83C84679AE23D
        4C04429AC4624678055ED8229C0D440C3E5534146AFD839E46021FFC12839F46
        F0B30A7C1AF4711D88352E8162D7219407B3C12AA52135FF12B09BE7077E2F02
        389D4EC3FAF5EBE1EEBB7F016B56AF8103070F202906F946E5B4C7E389625B5B
        BBF3A62080DA3A3F8790D7C1058663FC8D6119CBC03653E216B43101A6639509
        6195D3C87278905EC0561A41022F3D83E18B484906490AA342342A6089041928
        6710113D08E14929FA16B07E490009FEC06009737CA1FC0D1AF4C1B8EFD42FC2
        EC1589916981437BB741FD7167C3D4D3AE2095CF964F9B8924A5C77D7D7DF0F4
        D34FC2CAE79E83CD5BB6A006E840B9D00D994C16F70C6507DEB5D77E28F19DEF
        DCF0AA868E879500FAD6FD45683093C6F9A6635C0A96712182DF8C001A10D004
        A60C055240DACA338488A1C28765F921431D85A0140430544BBA7FBF43152668
        D206DD65AC0091034D3E09D47FBCB2138020F8A4FCFB074A90CF94A4E58BD940
        56623A1E1B219B9B05DB5E791ED6EEE983F16FFB4B38F5B4D3219148F0996A6A
        6AA02695E2BFB071E306D8B67D2BB4B7B7E3695CF406AFC0EE5DBBA0B7AF17BA
        BABA79AFAFAFFBE1C2850BEF686B6BB3972DBBFC8F5FFDEAD7DE3C04D0B7AEEB
        D01724CD13CD8471BE611BE723E0EF4042A40CCD13802200BE66AAB0606B5E22
        A02124E87A28F105A6039E21CECB477DDA3A0FD5914EC883EF05F8578BCA0414
        FE6A3A9828EF0E0CBAA8F84B6CF93425CC20D56F8CC17D063A81B9B06DFD8BF0
        D8CA57E0D11D2E4C6D99063366CC8471E3C6C2ACD97300C1C4C7E3A1A3B303C1
        DEC98270EAD469B073D70E58B972251CC0B4B0B3AB13F5C01E686B3B886122C3
        C522F218CDCDCD37EDD8B1EBBA372501C25BF7E7A0CE4859EF30E3E65908E459
        9842BECD885975EC09D81B589214126C391015F01E8EAC3558A16C43651281E2
        5338AD2C09C1E815821940A81EC0BDA2EC0984E50FA65D147C25308BB203983C
        806B816BCC865CE104D8B36D233CBD722DDCFD7C0FF4F60F402C1683C6860698
        3265325C70E1C5B85F04A9DA14EC42F0B3D92CCC9C391B012EC0F3CFAF464F70
        88C30281BF6EDD5AE8ECEC82C3870FE3FB72FC9EFAFAFAAE7DFB0E8CFBB32040
        7843ED5063A5EC93CDA475167A86254888D3CD983D0EFD86C100FBC251E98872
        AD410C553B8154D3AF40CAB0A10A4E867A0C6ADE6241A49181DBCFF8F8CB3050
        E2468F7406851B814F8B441101B8E1D385A23713FAD30B60EFCEADF0FC8BEBE0
        AE55DDD0DE29E681D4D42461C2F8F170EAA9A7C2BBDE7D35CC39FE38E84550FB
        D0CDB74E9F01A99A14AC7DE94554FF7DF8351CF6484F3DF50493818A447BF7EE
        C194B18F3306C771767EE2139F9C79A430F0A6244078EBFE3CC48CA433D71A13
        5B8280BF934911B79BD113180CB825B309E515FCE2935326855F855469A62A34
        C9E76AE91ABEC994CA1A5493A7181FA0717D03DD7F268BD63F486EDF15FD7E44
        00147DE95C0B74F71F0FFB766E8375EB37C2AF5EE881BDED3D28098A50934CC2
        D8A62698376F2E5C7EC51570C699981D2079F2F91C8C1F3F011A1A1A61E7CE1D
        D071E8100D0AC194A92DB076ED0BF8770619F4679F791A0EA1303C7488C8B00F
        4932004D4D4D3FABABAB7B78C284E6FC95575E793712E2CF9300E10D3D846337
        26E65BF58973CC847DBE1177CE366276AD3F2AA93C8222814E0C3B546B30CAB5
        85E0DC054FF42DAABB9FCBD89FCB7B2CFA4C39BECF237D28FA7A0726C1818EE9
        B07FF776D8BC7507FC6EFD20ECEFEC851CBA6C1BFF6E63C31898DEDA0AE79E7B
        2E9C7DCE3BA179D244CC904D16844D63C7B127D8B77F2FB8185A8E3B7E2E93A1
        88E71D839F7BEAC927A1FD503B0BC1175F7C01B66DDDCAE1208F7A83328A2953
        A69057F9F8F2E53FFDD15B8200E1ADE7EB769D3379CCD9664DEC3288D9179B09
        679A1993630D8E686235F472B46507338A0AF0B5358CBCB458F28DF850A474CD
        F59B3BB8F68F2074F74D863D6D53D1F2B7C0FA6DFBE0896D39E8EA1D40B2E4B9
        909342A53FB179029C8619C0E233CE4031D802D35AA7536ECF990085802CA67B
        0383FD2C120F1E3CC882B00545E34B1812A852E83871AE123EFEF8A3B073C74E
        38DC7B18DFC30347542442029CF6D7CB97DFF99F6F4902E85BC7E7C04E2D9C74
        86918ABFDF8C3BEF32624E43591F381A0942B505B9829921EB039EAC1A1A9EC1
        D3BBDCC20083EF15C5A08F27855FE7E1C9D0DE33137ADAF7C373EBB6C1935B33
        D0876E3B477301913489441C1AC68C81B973E7C2A9A79F4EC3BD3063E62C983B
        6F1E6A5C8B4380897FDFC57030163382EEEE2ECCFBD39C0D90384CE3B9264D9E
        027BF6EC86871E7A105E7EF965AE12D2A0519E0966C2A4491307AEB8E2CA495F
        F8C2172BEE52F6962380BEF5DED6988AB7345D6E249C8F2011CE41E00D5F1CFA
        A1409695150924010CA0024D391DEC47F08BB94139C34774F9ECEF9804EDDD2D
        E01845D8D55D82DFAEDC8D426D1F64E9AE21182E1C245E6D6D2DB4A2EB3FF1E4
        93398A6CDEBC0566CF9E8D6EFE7852F2D03AAD15E2C9048CC53040563F383880
        804E8681FE7E4EFD882CE9F420AC58F12C93E0E0C136AE15ECDAB913B5489653
        C3F1E3C73DF1E0830FBF33EA37784B13406D1D9F05A83BF7B845562A46EAE13D
        08BE0556B986203480366D4D3590B2F51B689106E4739EB07CB91EF0D6BD93E0
        50F7789A2F03D99AA9D05EA8878D1B3771F1866AF8269E3389C04E98300166CF
        99033174F74F3FFD346CDAB419EA10F805F3E7C345175D04C7CF9D875E2186D6
        3F0E1D4A91BD00957E0F1FEE41528CC5C72E6CD8F00A6A845E26480C3DC8AA95
        CF416757176CDDBA0D0E1C38802160D24FEEBFFFFE8F447DF7510284B6FCA30B
        16A246B81E2D7F993FE6608A22125B3D8D254079E008BD2C5AA5E1D7FD3D4C01
        37EE9A086D1D496E8818AC6985626A22BA731BB2B92CE7EEE9C1341328816051
        A1275FC82368AB60F59A352CE450BDC3F9E79F0FE75F7001C7F7FAFA3A38F1C4
        9351F0357028A0F3904720FD4062B01FCF4997346DDA74D8BC6923ECD94BB581
        97E1B1C71EE781A4CECECE5BF7ECD9F3E9A8EF3B4A802A5B71C5C99720E8B7A1
        E5CFF057303183E3032EA67F0308BE2B2778940A06BCB2B3157A7A6BD0328BD0
        654D86A29DE2020F5966CC89A163B1F9B3E4C2C9AA0BC8A0152B56C0DA17D7C2
        C1B636F63473D1FD5F78F1C5F8B9387A85A7380D7CC7E2C5301EDF4FE4983173
        260B441A22EE43B14769E4AC5973786C8008B10FC3CC8F7F723B01CF052424D1
        D7DADADABE1DF53D470930C4565A794A9D07C6BFE0CFF401AE3A6AE0B3EBCF99
        EC01A82E94CDD9B076DB0C18CC24218F62F1B035114A5C4412033A8204097EDC
        D3D3C3AE3F95AA85EDDBB7C14B6BD762EAB60DCF5560529C8E6290B280A79E7A
        8A4BBE0E7E760EEA8233CE3C13965D7E394C9F3E83C5DF20C67E22D044D40439
        F40A7BF7EC41F5DF0BB7DC722BECD8B183AD9FC24D3A9DFE444747C7F7A3BEE3
        28018E622B3D77CA9568EC3F44023493DBF5A4F5F70D5AACE4DB3A4C78EAA566
        04B91EBC7823A4E313CAE3067223E08940ED870EA1889B040DE8CE49AC9157D8
        C0033BBB59ED93F83B7EDE7C74E1EBE091471E412BEF8064228121E044F8DB0F
        7C00969E7D0ED4D5D532B8A4F2C7600641B5FF3DBB77F1757DFFFBDF87679F5D
        E1FF3B8D14E2E3F7A316F859D4771B25C0516E8515278FC59CEF3624C03524FE
        72051355B609CFBC98877B1F2B602AD708C9F133A1665C0B8ABB1A8C1EA6045D
        FCC405B4EE3DFBF6C20474E764E55DE89EC74F98C044D8B7772F02B81B33823A
        1484C7C1DE7DFBE0C1071F841D3B77F1E04E73F304B87CD932B8F8924BD133B4
        7248A09043E1847A057A310C1009EEBAEB1770CF3DF730E8B4110148182201AE
        420D706FD4F71A25C0ABDCF24F2F3C0F31FDF2C14EF3AC3BEEEB771EF96317BA
        FA18B8A966689ED482F9F914686C6C40AB4DA24770B81053C4B470EBD6AD5CB5
        6B46D0F7A3326F9ED0CC603A8E23067132599832752AAB79CAE75F421147357D
        3ACFC9A79C0C175C7821C45113D4D5D5C1BC050B785CC0C1F353F999420411E6
        8E3B967378A1704384A0F82F09703E868447A2BECF28015EC3D6F979802FBE7C
        E2ACA736B43FDAD53BD05AC2989F4AD5B0482380C78E6DE2DA7D5353038341B9
        7D5353234C46D7BF6DFB768EFFADD3A6B126E8C3982D44DC6CF6182FBDF4123C
        F3F433B00FBD00798FC953A6C0D2A54B91548DB07AF56A4C37F33063D64C54FC
        D330555C002D48224AF76EBEF966AE10D2DFA3EA21593F118076F4228BB76CD9
        F25CD4771925C09FB09D70C2BC19FBF71F781C2DAC952C9D62355928A56D4484
        89139B598993729F3EBD1505DF0E1EF06969998AF1DE446BEF8322660BB367CF
        614BA7DCFD10EA823508343DAEC390B01063FFCC59B3601D12E3D9157F847426
        0D2D535BE0DC73DF0973E72D80F51B360082CB6DE344181A28221250AD407980
        783C7E02922772A5915102FC89DBBCB973661C6C6B7F02AD6C1A593055F7C83D
        5391861E37A1E59E74D242EEE2A5AADF71736673AA77185DF56174F70B16CC87
        24BE9FC20059AE83A270D3C68DD08D29DDE4C953A015B301D2078F3DFE38E6F7
        7B59342E5AB4082EBDEC3258B96A35AC59B386051F69010A27043E95936923F0
        972C5992BFE1861B4EB62C6B63D4F58F12E075D888041D9D5DDFC2B8DB8344F8
        30029148219875E809E6A1AAEF426BA6F83F67D64C4E23BBBB7B6010E3FBC285
        27B0C552334712DFDFDA3A8D47FB0E1C6CA3F22D8791F6B636781CC1DFB2652B
        377B4C418D71D965CB58F1FF12051F358B16B8CBC860E039DDC41D0147AF33DD
        FBB77FFB37FA1B0B909CA3043816DBE4C9CDD7A30AFF720D5A350DF2B86E8987
        74674C9FCEC0F40FF4731C5FB4E864F60EBBF7EC65B13807C30079104A135B5A
        5A601CEA096AF87C79DD4B5C24A274B016D3BFC5679CC9990281BF6FDF7E76F3
        24F894F5AB9DC6176EB9E516F644B8CDC3736F8ABADE5102BCCEDB94C913BFD0
        D7DFFF1D0684D64A424BACAD4D017904514072E1C4134EC00CA0853B7BA996CF
        61012D782F023A15451F8DDF9330A43230798AB52FBE08838369168A3462F81C
        BA7ECAF5C9FA294D24FDA108404722D06DB7DD467D81EAB2E6220122179E1C25
        C0EBBCB5B64EBDB0A7E7F003689586498B25902E2080181C9B9B3E66632820F0
        4AE81D68D087DE470271F2E4C90C5EA158E08E1E7A4C6A7EC7B66DEC1DA6CF9C
        C51EE3C187FEC0E420F0E9754500DAA9C8F4831FFC00D3D1A9FE35A1403C69DE
        BC79EBA2AE7794006FC03663C6B44508CE3CB4EE9998CB7F155F8AD1885E7D5D
        3DD709A88843C02D58300F63B5CD29DF14047FDAB4168EF3DD2810672149A8CA
        47C3B9D4FF474D23DB77EE82DFDCFF5BCCF50FB3CA27AF41E7A1784F24A0B041
        E0D370B2DA50241E58BE7CF9F1F8FA40D4B58E12E00DDE9A9A1A7E8422EDA364
        9D9405101108504A056938B80B056033A68B27A02720500FB6B7C1EC99047E03
        A67C19168914C7F7ECDD07BFFECDEF385B705DD194AA08403B5514BF7BE38D30
        0FCF437F8388B571E346EF8E3BEEB8E2A69B6EAABAB4CC2801DEE06DC284B13F
        452B7EBF1A10224BAD4DD530B034F84305A205F3E7F2C0501B2AFEB9738F67F1
        4815C15822CE15C3431D9DF0E0C38FF2C00E6D24FAF4994363EAEBE1EB5FFF1A
        771151B590461EF762CA88E0FFD38D37DEF8CDA1AE6F94006FF03665CAA4ABFA
        FAFAEE46C02C76D714B3510B9085523B18893EAA20F6F5F7C142148753311450
        0D80068E2815ECECEA866757ACE2FE41DAC8FA75025098B8EE1F3FC3BD829C06
        E24E9DC13FFFF97F2E479DF1214C0387BC05CD28018EC1366346EB3CF4D64DA5
        62694ECFE1C3B7A3EB36290CD03C803163EA194C1AEE9D7BFC712C0E699067F2
        E48970E0403BAC5CFD029F8342086DF45E450202FF631FFD30670DA20610E75E
        C05FDD7BDF2FF13CD7602670C4D5C64709708C37D4044FA026389B3C01592B85
        046A0927D15793ACE19471F6EC59E8C2F7C31F578AC53FE83DB493D8F3633E82
        FFBEF75E8D5E623C3787D0B908FC871E7EE467994CE65AB4FCA3BA21F528018E
        F186045881626F31093802942C7B4C7D1D5B33CD0AA2B2F1AE5D7B60F5F36B41
        BD472700ED1432DEFDAE2B798C4155FFBA313378E6D915DF435DF139B4FCA3BE
        0DED28018EF1366FEE9CFF8520FD18C14D20111C06D6B658C82D3C61016CDB41
        CBC06C0900AF13A0755A8B77D9A5171975985190D593F57775F70CBEB4EEE54F
        DE7CF32D77BEDAEB1925C0306EE79D7BF65F2000D72201524D8D4DD9975FD9B0
        65D7EE3DEC1510702F4C8089CD13F257BFFB4A882712265ABE418A7F60306DBF
        F8D22BB7A3CBDFF55AAE6194006FF16D94006FF16D94006FF16D94006FF16D94
        006FF1EDFF03035BE6435B23EC6C0000000049454E44AE426082}
      Stretch = True
    end
    object LinkLabel1: TLinkLabelEx
      Left = 16
      Top = 56
      Width = 289
      Height = 116
      AutoSize = False
      WordWrap = True
      OnLinkClick = LinkLabel1LinkClick
      Anchors = [akLeft, akTop, akRight, akBottom]
      Caption = 'Tip of the day'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'Microsoft Sans Serif'
      Font.Style = []
      ParentFont = False
    end
  end
end