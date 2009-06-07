object Form2: TForm2
  Left = 400
  Top = 137
  BiDiMode = bdLeftToRight
  BorderIcons = [biSystemMenu]
  BorderStyle = bsDialog
  Caption = 'About Graph'
  ClientHeight = 307
  ClientWidth = 313
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  KeyPreview = True
  OldCreateOrder = False
  ParentBiDiMode = False
  Position = poMainFormCenter
  ShowHint = True
  DesignSize = (
    313
    307)
  PixelsPerInch = 96
  TextHeight = 13
  object Button1: TButton
    Left = 111
    Top = 273
    Width = 75
    Height = 25
    Anchors = [akLeft, akBottom]
    Cancel = True
    Caption = 'OK'
    Default = True
    ModalResult = 1
    TabOrder = 0
    ExplicitTop = 299
  end
  object Panel1: TPanel
    Left = 9
    Top = 8
    Width = 296
    Height = 254
    Anchors = [akLeft, akTop, akRight, akBottom]
    BevelInner = bvRaised
    BevelOuter = bvLowered
    ParentColor = True
    TabOrder = 1
    ExplicitHeight = 280
    DesignSize = (
      296
      254)
    object ProductName: TLabel
      Left = 144
      Top = 8
      Width = 77
      Height = 31
      Caption = 'Graph'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -27
      Font.Name = 'Times New Roman'
      Font.Style = [fsBold]
      ParentFont = False
      IsControl = True
    end
    object Version: TLabel
      Left = 144
      Top = 48
      Width = 51
      Height = 13
      Caption = 'Version %s'
      IsControl = True
    end
    object Copyright: TLabel
      Left = 144
      Top = 88
      Width = 68
      Height = 13
      Caption = 'Copyright note'
      IsControl = True
    end
    object Label3: TLabel
      Left = 144
      Top = 64
      Width = 39
      Height = 13
      Caption = 'Build %s'
    end
    object Comments: TLabel
      Left = 8
      Top = 144
      Width = 280
      Height = 52
      Anchors = [akLeft, akTop, akRight]
      AutoSize = False
      Caption = 
        'This is free software, and you are welcome to redistribute it un' +
        'der the terms in the GNU General Public License. Please send bug' +
        ' reports and suggestions to the programmer.'
      WordWrap = True
      IsControl = True
    end
    object ProgramIcon: TImage
      Left = 8
      Top = 8
      Width = 128
      Height = 128
      Picture.Data = {
        0954506E67496D61676589504E470D0A1A0A0000000D49484452000000800000
        00800806000000C33E61CB000000017352474200AECE1CE90000000467414D41
        0000B18F0BFC6105000000097048597300000EC300000EC301C76FA864000000
        1874455874536F667477617265005061696E742E4E45542076332E3230599F76
        51000033D64944415478DAED7D097C1CD959E7ABB35BADD62DF9942C5FF27DC9
        1E8F3D3399CC244C08242C8449588EB0401242B85936CB5E6465425896CD0696
        10B20939D8840418820708994C268733B1E7F231E34BBE65D9B2ACC3D6D9EA43
        7D56D57EFFAA7A5DAF4B2DABE5D169CFFBF9B95AD55DAFAADEF7FFCEF7DEF724
        F646B9AF8B34DF0FF04699DFF20600EEF3F20600EEF3F20600EEF3B2A001B06F
        DF7B9463C70E1AF3FD1CF77259B00020E2BF930EAB08009F99EF67B997CB8204
        00115FA3C34DAA9F26007C6CBE9FE75E2E0B15009FA3C307A9FE3901E077E7FB
        7966AA1C604CA743B3C5D83AEAF80DF4790DD510D514D5B68F323636D7CFB4E0
        0040C47F3B1D9E73FEB2BE190C0EFDB2EF2719AAB1C3870F67E7FB59A72A44F0
        2A22F68F50273F4C7FEEA5BA9339042F289624B5DF686A7A67D79A35008049D5
        102BBDEBAC3DE3820200117F051D5EA3BACCED9A2E02C0BF137E92A31AA17A93
        3A2531DFCFEB2F447015DC4D1FDF411DFB245E89AA36D5755955BDFCCA430FFD
        AA29CBE3F427AF31AA71AA297A576BB69E79A101E0BB7478C23B63450281E15F
        90240B5C61B91D3344B58B3A253EDFCFCB4B1B63D57400507F9E3A74371DD5E9
        5C9F0C042E1DDBBFFF8F99A3025047DD3A8277BE2F0040C4FF793A7CC5773A43
        00F8354932D3CC11FD51AAB798038039D797FE42846FA4C307A81361A7544DF3
        72CB94A434897F73ACB2B2FDECCE9D4F3187E0C3CC01F9A05BE3F73C0088F810
        F99D6CA27EB4743DF227B29C45A7C4DDCE8177D0419D32325FCF4B84AFA4C301
        EABC0FB012086FC872221D08F48E97950D25CBCA228970389E088552195DB74C
        459173AA6A1010F06EB7A9F6BBB5CFADD17B1A00447C990ECF527D7BB1EF352D
        7A5051D25DCCE10E740C807299EAE06C764CB1423A5EA11BBE873AED13CCE1FE
        A2857E6312717B2255551707962EED1CAEAB037801EE32AA41E6D80550130A73
        6800153740B5876AAF70441DBBD701F01FE9F0BF27FB5E5563475535758939DC
        D1C51CE2A3F6CF250088EB1BE8F025EAB01F65C5FBCDCAA86A6FA4BAFA74FFF2
        E59DA3B5B5505B61E6103EC01C1790135CF2B501E3763200DCBB128088DF4A87
        57A9CA93FD4655131DAA3A7E813E7653EDA00A3000003DD431E674EED772802D
        931C820C814DE95FB6E30F5972AAEB88F37F9C0E7FC5F2DE8957D00C71FBF5DE
        152BBEDFDBD8D867280AD4430573EE03A2ABEEFB818826F3DCBC9C50011600FC
        9650018881D93676E70D00AEE83FC91CDF78D2A228C9414D8B9F619EE8E700B8
        419D53749C60439BED8EC1EF7E4492D87EE658E6AB98C381BCC0A0BC4D64B966
        59ECBB92257D558F570CD69D6A35B9DFED8AFCDFA34EFA9FC5EE43EE5B6FFFB2
        65DFEE5ABDFA26E9727802207E3973381E8407D1416018B069B7A6849A11BE83
        D50F3B6084795E40849E25359B74984F0020C4FB91A97E27CBE9B8AE474FD1C7
        2BAC1000D7FDC1A09636D2CB12FB20BD148247CBA7F97E516648CFC9E9C02797
        1EDF77B2B1A7C75AD7D9F9456AE0E7FCED105533A3D5D5872E6CD9F2724ED340
        F85AE6887BE877800CC00461215D10AF88BB95FBF838CF3D1B5E63C2EFF2BF9D
        ED80D7BC0080888FC8D80BEC0EA23FFF80528E5CC151488ACB4205083AA973D0
        89E0F8167A933FA28F3FC68A44DAA6552C36AEA4B42FBDE3B3B935E1A8F5A3FE
        AFC1F5D7D7ACF95ADF8A152010EC027801E07A101EDC0E8E8DB915AE6AD4AD7E
        00700990658512829FB3D5C374D5DC74CB9C0380880F31099DBEB2A407940C23
        101839CD3CCEE7B5A3FFB1C341E2C68F9198873B16B8634316B3DC372EE99D6B
        C8DF78E89F88BA435E0BE3C1E0D973DBB7FFF3782804A283F8D0F53A73443D88
        0A42F3008E2DC2990302CEDD09E6111FC406A179C897DB0876992B03773E0080
        60CFCF977E8565050243ED446400C00681A5E4AEDE7EF0F88356204B6A9AD517
        BF4C4A29C9E045753CD4AB26CA236A3298914C4535D56C452E98AA35CA134B33
        55D1664B35CA26BB7368CC01C112323F47AAAB0F5FD8BAF508F9ECF5EE3DC1F5
        90603C4005FD8DC0CD90FB990380737F9279DC0D829BB319E32FB5CC290088F8
        6FA6C3B4DF3A1018BE244926A4C6E54CD5D8C0E8D6F33F63EAD97D457E6A4969
        FD5270A4F695CACE75E7E49C0A22718BDCEF7F2B048640ACF9C6BAD492813566
        205B5475048977371EAE3A3B14DAF103539621BDC0FD000D0F4D83DBB9050FCB
        7DD03D07CEE7845F504417CB9C0180880F6220DCB996FA0E042062584414B9C0
        5823910F3128DA0696A645BB6439D73EBEAC3F33D672F5ED44BA0A5FF3A69CD1
        2E957737FD53796FE31059F435CCB1C8B90FAE0A6D4AEE67D93DAF198154D85A
        72E681A135742C6295485935597B76FB4B7AAC32E35E03FD0CB10E82C35747C4
        0E412A70FFA8FB5D9EF0731DB09A4E995309F0D8638F81F0B096A13F9B72B9D0
        3B73B9F20FE71F46CA91BE1F05A7882368E8EC1B631BAEC8E3CBFB57D3131792
        2827F785FA977FA5EADABA9BC4FFB0C6ABDD7B804BF9A00C08E0D7B378771B88
        2B7A7BD7ADBB7EF5812B0F3276E609FA511110A8F1F258DD991DE7E5AC0EAE86
        6807C1AF332738C5890FAE8754807E5FD084CFF7F95CDE8C000082807B97506D
        4AA7ABFF9B65696FE1DF93CF9F249F3FCBBCF068C6528CECF0B6F670B666ACD2
        DF1E11E5D99AF35BBFA126CBF05D1DF3C43337CCD096E87FF3CF3C7E20D70E0F
        376DBD70E19714D3D471D78BFB193BFB43747365E2F397F52DBB55D5B1A19D24
        0C22753C2E0110401240D7DB5CBF1808CFCBBC01C0B2A4E674BAF64BAE0AB08B
        A68D0D284A86FBBDE95C30991DD9DEBECA284F16186A92A144CB7A97FF1D717D
        3B73C054C73C8B1C05A21A9C086EE52E19B7C0F19D2D0DC2B158CD8EB367DBF4
        5C6E156F1BDCFFDA1392D9F1501139604A66F5C54D27CA06971C679E3B0A000C
        BA6DE7169A8E9FAACC1700EAB3D9D0A38651FE45EF5B334BEEDE5549B299C7CA
        5444ADD1EDE7D693B157E0DEC9A9604FEDD96D4F69E3E5E0647C07910F5D0F82
        013C20344431C43437C6A2CC0700C9B2D883C78E7DAC2C9D7E97D83EF9F9E933
        AD5B3ABB1EE95A9BAD8A06FDEFA0244223F5A75ABF4A06268C521100B33A7163
        B6CA7C0000FAB99AC4FFEF91F8FF75FE9D2C6787743D8258BF9CAE8E8446B79D
        DB442E5AC16C1A2D5275BAB67DDB0F64430561407C086A703DD405880B628BEE
        9808004804A801DB307BF8A597DE439CEF9F7FC03AD6AF7FB177E54A29138E35
        8FEC3AB3929E61421F856E341DAABABEF65F992305AE314705240800B9B9ECCF
        9928F3010088F3502A55FF6DBA7D7E1C4055E32754353992AA1DAE8D6CBDD84A
        BA5F9C5563056F2DF976CDA5CDE79813768547C1AD711E4605E14108B86383EE
        DFDC15CB131E5CDA462A835E1CC1A58221DD6845C5C593BB77BFEA9E6F88AEBE
        DE9868EEAEF6F7929CD247EB5FDBF31925AB43050100F00200B6E4629302730D
        00DBF522EEDF44DC8F8E7639DCCA11F73F9D69B8DD10D972F15122BEC7F916CB
        95DD5AF654F5E58D10B7188D030002CC31F2405CB85D203C88C07D719C03D7F3
        70ABE11BE0F91CBDF8FBC567CBA8EAE0E9D65644F910E4B1D58AA1672A86779F
        6C318269DDFF2EE5379ABE5F797DED33CC19A3E8628E27105D6C52609EC6029E
        FC10F1D167F30F21193DD6F28E9788F3DF45C4F774BE29E5CA6F367E893AFA22
        FDB594391138E87B101F3A1D5C8E8EEF619E2B1671BF2BEA8A11F7FF38BDF441
        26B89A98C071A5A5E51FFA57AC802A815109E962CF438C3577AD8CAFBEB1C91F
        4256C6CB86EB4EB57E59C96AE799334C8DE16ADB185C4C5260BE06830A267F9A
        958317A30FBED46CA9392F1A67D1E9ABEB10D801E743E423B803DD0FC2884118
        4C1103E7030C5CD7678B0DA210F103F4C2185ADE289EC79CBC53ADADC798333E
        011505F0D8317C434F4B837B5FFB294BCBFA834FACEAD286E742B79643B4C016
        E864DE0C9E452305E6612CE0DDC4C91238C6EE50534FB1E8C3DF4B5BC194C7F9
        16B32A3BD6BF50DEB712BF0321036E45C7F289A1E038101F6158E85F1E7D2B1A
        803960376B8FED7F5C3C9F5394D8E95DBB9E8E87C310FBA870112149A0520864
        567A64DBB977A6EB477EC8DFA63E5CD35D7B6EDBD39225F381AA2EE79AC5E311
        CC03009E24B74BFE677CB66483C5F6FD801955A3DE0FA8DB2A3AD75E0AF73475
        312F7883C2D70480303798477C5CCC0DBD4963EDC4FD2D74382A393644BE7437
        363E776DDD3AB401D10FAF029C0F705D650E4727934B065A229B2EFD2F265B05
        2EA99453B2640C3EAD26CBCE3277849239EA283AD964958556E643023C45B7FD
        697C8EEF3CCAB2CB7B0ABE0F77ADBA5DD1B5064416C7D5F119440241BA5821F1
        A1EF33538D9B1300BE4A2FFB5EF15C2A10E83FFEE083CF9AB28C05297C8916EE
        7DC5ADB84FD2D4B23583BB5FFB8459967EB0A051026BF8DA9A572B6EAEFA01F3
        86A9795C20BD188242733C1AF86EE87212975263B2A59DA5D65D2EF8BEAC7F69
        8CAC7D12E712273E9F228523880F978B139F1B7B99A9C46D9BB342E788E4450A
        413B837CFE67FB56AE8464817D01000DBAF7E0DC6C4B00AA9523DBDADF4F6AE0
        BFFADBD647AB076ACF6E7F86D4C045DF75B33A9F7FA6CA5C03E071BAE5F3E9C6
        EB6C7CEB49FAE8F54F60B83651737E6B4232656EE4710B1F15441789CF397FCA
        B8BBEBF67D9F5EF4CDE2F931F2F949F79FB46419DE058001DB02AA854F3DEB72
        EF053056C457F63C145B77EDABA4060A8C415203B986130F3CABA483EDCC9302
        B87664311883730D80BFC8D60DFC567CCF4B640678125B8D8513F5A7778E4986
        0A310C8EE3C4EF722B880FE270838F5BFA5372589BB34EEF9BE239B87D2777EF
        FEFB584505068F50D3EEFDC0BD2020D7FF90320820850C2DD338B8F7C43F587A
        6E47C10D2CC9226FE064E8F6B25798373E80EB61A8A616BA1A98C3F900EF568D
        60E274ECA1E7B75A8174FEBC9CD652F527F77429E940BDFB3CE06E1003040741
        AEBA9FFBD934381FA5CD093B7F971ADD2F9E8F54559D27EE07C782FB119C02A8
        A0BB41401ED88127804999E6638F3D06095137B0E7D5FF6E54247ECD7F9F60FF
        D21E525D8724268953D620AD620B5D0DCC19001E7CE85DBB637B5E7CD5A81BF2
        EE694966CDD96DAF06476BA183E18281B8E0724E7C4E0C1E6A85B55FF2702B01
        E057E866C830921FD93314257E7AC78E6FC52A2B21CA011048939BCC5B73007F
        DE8EEAB9F7E211CC8A912DE79F4C370CFD15F55AC11885120F251A4EEEF90EA9
        2F71DE22DA1959E8DEC09C0060C3019875814F59C1F46F8AE743379A8E575D5F
        0B71002B1C2E18B88E2F0001F1C1957DEE79107F4A838F1722BEE6C6FBB788E7
        6F37349CB8B8650BEEC1E712C2F0BBCA3CDD8FEFA082F2BE3C01008760B27E70
        6364CBC523640714CC4D90B2AA517766C7CB5ABCE234F30080F66E511B99B9E8
        E3BB2D7302809636F63392C4BECA848519DA48CDD5FAF6ED27490A602C1E1200
        BA1F9CC889C1F530880FA3303D1D714A00F8457AB92F89E70C594E9E6C6D7D26
        E1047D10F103A84070916B2181627E030E0359D9707CD9F0CE33072D2D37613E
        62F5F9CDED65834B30BE214A01B4BDA043C3B30E0022FE5A223E38236F3D2BA3
        B5ACE6ECCEAF6B9689FB430F23F60E42F35936203E8FAD4314A7A7333F1EA958
        A8C73B25DF68DFAD254B4E5CDABC19A0C2041288E6DBEEBD38C7028023AC08D8
        080078D6AA5BFB8E7EC22A4B7FC07FCFD0CD953D559DEB5F628553D761BC46EE
        5B0010F17183EFD07F6FCBDF30A7B28AA38FC74359E35F25C9E4D3ABE16A899C
        C82D7E58E1A9E92E8E68731235148CF59BB29C39BA6FDFD3195DC77C4470FF98
        7B1F3FF7C78BDDCF5503A1A1D6931FC856C5FEC2FFBD3E5C33567B6EFB0B9225
        891200AA6C7021DB01B30600E87DCB620788FBFF403C1F3ADFCA823DABBA0281
        08967BC10503770C31CF05C3117A1F167F72BA9DE7AEDD7F895E6C9B787EA0A1
        E1D40547F783FB21DE61E8F9B91FF79C34824720D086B7B5BF2D533772907AAE
        609A9A920C661A4EEC7DD135042F09EDF62EE47C46B30680F56DEC8765C95EF7
        9FD7FB7A4F332B3FFF005395C451554DC6DDEF20E26FB242B10940C02398F61C
        3B02C087E8A53E2B9E23CB3FF9DAEEDDCF8E8742D0FD185104F7C3C0E4861FE7
        FEC49DA40DBC81C48ADE35D1F5574F935F112EE8C8AC6AD69F6C7D4D4D86C480
        105FC43AE50AE4F92AB302800D6DD4C912C300490B3FA7C42B59C52B6FC5844E
        43D7475F91657BAA153A464CFAC00D27187D99BB207E15357A843E16046B06EA
        EBCF5CD8BA15E29E733F240C973822F7DFF19EB003E22BFA96C5D7763E67A9A6
        3F204476CDB68BE4D2724F804B01003ABA500342330E809603D4A6C53E49A2FF
        B7BCCE2187FBF8634C1B6DC0DCFFB14060F40AF344BFC889B6EBC4EE723895B4
        0E82349F16DF2BA72889333B767CCFF5FBC1FD11E6057D3881A6E47E5E0804A1
        FE475EFC06D38CB7FABFABBCB2BEBBBC6F25B29CF9ED8091856A08CE380088FB
        FF2DB5FAF74C08BE04AE6D60A12B0EC3103DFA346D9CCFD5E33E7F7EC52FBBCB
        E00971BFEE4EF6D8249E27BFFF34F9FDE0700C03DF35F7F382A8E0AD875FFEBF
        969E9DE80974370E545D5BF72A2B9400B8C7EDD95EE57BB765460140C4AFA016
        312A965FF90B97AFE2C4E38C8C23FB6F5D8FF4C87216C111D105130751EE6A14
        ADCDC9D6F505F11C2CFF533B773EE7723FC6F223EE3D44F15C32F7A36075D3AD
        FD473F6205D37FE0FF2E30501FADBDB0F5042B04006AEF421D189A3100AC27D1
        2F5BEC73D4A297D993F422F4BE1AAD716E26195912FFC35869CD0AF3FD881673
        7ABAF726D11F20C45CF5FBFD83757567CF6FDB86FBC0DD84255E94FBA713AD83
        2178FBC163EF37CB527FE55FA6A647AAB086F035C95444E2E3F38D851A119C16
        00F6ED7BF766E6186EF6B2E763C79ECE730D71FFCF526B7F2BB619BCBA99955D
        DD9ABF5E9653295D8F81F808F070BD2FEACAA1BBE19462513F8CF81DDFBBF79F
        93A110447F31DD5F92E5EF2F30040777BFF6D65C65FCBBFEFE93527A6EE9B1FD
        AF49962412DF96340BD51328190044FC9DF473183831B733DDF5EFD690A9A56F
        8CBDE59B3FC7646B05FFBD3C5619A93AFA4415B3E4FC3D346D2CA9281974040F
        C0889C02E24C3B251A717F255D70C24DBE9C2F23353517CFEED801153399E5DF
        CDA6A1FB7941406860CFAB0F19E1C4B7A9F70AE606C8E980517F6AD7792515E4
        AB86F8BB5DA57BC4F6ED7B0F7E86C4586717CA3E08D30100E27A1F2DF65D62CB
        49965975CD3B41A23FFCCA5B0C2D5A2B2CB1B4B0F8334B6A20214926E681D105
        D665F216CED1F1B42CE740ACF1BB70FD2670BF2949D993ADADCFC62B2A101412
        FD7E91FBE16D4C8BFB79D9FE1B75ABD275C3E7A9F70A630139C5AA3BD97A591B
        2FC754711100005C24956AA047B5E8DD25958E9FA4734FE9FAD875B289ECEBE7
        C353280900447CFA9D843C3DBBFCDF65EB06587CEF918273C1CE4DACAC635B09
        2D5B7CEA1738D15DCA65412F13774AE05088E831B192DAC9DB086DCE201266FB
        143C57A4AAAAE3F4AE5D20008FF98BDCCF630DD3E67E5EB6FF7ADDB274DDC829
        92788569E3887C75A7765DD3A355FE6010EA3001006168BC1F5FBD4C92D0FA1E
        B9C67F4EAAF10631075FB93C6789244A0540AB2BFE0BB365C9268BED7BBE6056
        AF12AB6495AF3C01137CA69E913AC41240205135614374EFB8726CE98AD1DE82
        2C5E06B153FBD66D8722B535E07C3FF703141055083EDD15F7A36CFBADEAFA4C
        F5D88B048082F505247AACEA0B9B7BCB861AF82C617E4F3BBE914AD5EFA147C5
        CC217F32697289CD8304808324118E48920520406DCD3A104A05003DB8F5ABCC
        89EC61FA34719E54995C7F3E945A7FC9FB2189FEF2530F317D604529CDBEAEA2
        1A39B6FFDC21164EC50ACEF7D635E5CEADDF1363648981A3A8332155BAA8927B
        2A5DB22CB9C3B2941ECB52ED61669228D3D6C5181AEE7BF448BBA4589BFCDF55
        5EDA78BBFCD63200C09FD4AA9700F02E7A86AFB1C27C856249396AD1F833C2F1
        614D8B434D413FCC1A104A02803B140A7D0AFFBE893AB0251D8EEF1EDD73E297
        880BF26D047A1B49FBED312D1373FBA4191301C5CAEAFE2B6C53F7D982733959
        652736BF998D856B27BB8C38DE4200CA5D312C513531FCDBE35509EA02466E8C
        7929DE12A2C7B3EDB7AB43D9AAE87324011FF5DFA0A273CD70F8E6AA6200E826
        00FC2EB5FFF112FA1D09484F92ADF41592085F2540D87320D92C641D992E00E0
        67AFA2176F1EDA7DF27772E1449E03C800321A8E3F788D2CE1403A5D4B4091F3
        2827F7CF22238F99A66222371371A165D9B89114F177A51685B8FFD133CFB160
        B63089664FC36A767ECD1EECC0F13ABBC516C120BC0B021B28B0479CFCBD92D5
        1F79F45B1FB642A956FF95E5D79B472B6FAC166D807C665302C05F525BBF32CD
        6719A07B7F9AC070081953A91FF1D2330684E90200B27DE558CB959F1C5FD99F
        5FDB6FAFE6B9BA6E24DCDB98350C3D94CD5615B847BA1E49924893989B4FCFB2
        20EAE40C0121837C1F74043874922C481A15A4BF5558CA04128DFED6FCCFB9B6
        F722DBD073BEE0194D12382F6D7F8225CA2AD95C9478EBCB2CBBB46FC2F9E0F5
        B599F0D5CD20DA4DC26107540FBDD745D3D448FD289FA15779CB5DDC8E3940B0
        9E2783F19381C018240CCF74F2BAD4C3740000A23664C3B1E6E15D67FEC6528D
        7CB8578B56C4EA4FEE86615596CD56D4194630BFC813891E757D74987431E774
        20584C96282239FF3C04028500204342D0912A002207F46426F4C8D9C335AA51
        38C4DE53DFCCCEADDB7BD71D314D62B0C4CEE32CB3FCE6846FF41B6B59F9C5DD
        E26F4124AE76B02E3250E24D262B86E339189F5594D4CBAA9A44BB776D274C07
        0098B953757BFFD13F3483696F6D3D66F6B66F3D1D1CA993885065994CCD5A10
        8B7FEDE6FA85F8E4091D78EE5C1C01028579B9FB34F788CA53B97929D6E9062D
        1D1DCB57F6F73788CF477EBFF5F29E37C5927A5873A487225B768A1F09AA46C2
        5394FAAEA596F8B6E32CDBD83DE1BCDEB39A959FDB33D3B72B568879AC974835
        FCA3A6C50E9284B5674C33070825AB87E9002030B2F57C4BBA6108831D791407
        06EACFD55ED80A37AB96C45C752653BD55BC961EEE3A2115BA144400E78FB895
        2FEDE200D084A356E46FB56A6CAC667B7BFB0ED528C81EC27A972FCF746CD860
        BAEF63A78423C2536748A466A41C56A1386A46266028502BBA0314FC0D3B0481
        99E919AD13825FFC7DFB9A58F8ECBEE934F57A8BE90497CCFF27CBC6D7553571
        99EC049E14634A20940A0096AA8E28233BCE3C43DDF423DEADA554C3AB0FFCB5
        3A6EC7DBABB3D9708B6194ADF3AEB420FE5FA307E3F17D882B311F3E508B8E9F
        8AF8F6DFBB4F9E7C67652CD62C3E5B5AD78D53BB768DA7CACA7892289E70991F
        010C2E51C4CD1A483CD80090A1A89CCF32D91D0A5559F30104AAA80074E30480
        741100A8B756B08AD30FCF250084620D52FD1A49832FE97AEC22726DB129EC84
        92E5544B1B7B2B09D343E2B960FFB28335973722AE8F8858593A5DF338F9D779
        1F8C0C96A14060F4B84B4CC325BA1BE9B3DDAE5201A0367577EF5C7BFD3A52C1
        17706A775353E4DADAB51C609028625A38B84F56B1F67C556185AAC705880450
        B800415514C73651D4F8A6F6507AD58D095BC269B757B0F2D3FBE9E259F582EF
        542C17084708087F4636C2453AA21F8A06964A0240CB015BFFBF2C0953ADE4AC
        DAB7F4A547B0C36713D56AD354CB49FCBFCD71ED9CA228E3A7352DD1E5766C82
        79F3FEF9722FAE1A8A1128FF99FCCAF003274E7C4E338C82E1DE6420903CDDDA
        DA4B9E27266802ED7CD3059EAC39EE0260B2B6EF785F1F58B8AAB26D93C8862B
        35C915FD056301360086EAADAA530F920A229C38760873ED104892B95E8E0FF5
        F00A198C5F20667C81DCC81E82318090CF9954D2036D6863FF9E7EF97FF2272C
        6684AFAFF97245F72A743022835A361B5A6918E5E21A3C4BD322CF294A968F83
        8FB0C20920375C5058AC307F2F37FCEC8E27B653F79E38F1BE50323961D78E8E
        F5EBDB7B57DACE48D02536DF75EB96FBD90F8052897F476058F46A91CD97D6A7
        960E2CF53F5360B8C6AA6DDF91DFF59340007BC4CCE5C265A6A94F483635FBC5
        4A20124AC6E2D39A16FD6B52C750C3F954F553028044FF52C22DA670E7337AAA
        89D0D986137BBFC19CC010DCC33489FF3713ADF2AE21A16E94F4FF77087121F7
        66208A3807A0EF4E933FDC79F8D2A32FBCB05936CDEF49C2FD51C683C181137B
        F7BE4A121A000468F896720056B77BBF18F3544029DC5EF277C3DBCFBE395337
        BABE18006ACE6DCB900A10377FC852FF2CA5FE79BD2EE054C436B06119111971
        886112FD1789F0FD641422DB7ABFDB473CCA697B615303E000FB14FDC85BD367
        49E9EA0B9B3F5736D8808EB057F49A26B179A6E649D1C795E5CC255D1F4344AC
        CCBD99B8F00392A0A4C91F6D8C3D43F77F67C16B923CBBB069D3A1C1254B4010
        0010DE458FDB2E46FDAEF900E09700AF8BF8A843BB4EFD74B63ABADBFFBCFA70
        AD597B7E6B5C3265BEF58B6D8BA452757BE17DCC20B1999355D5B033AC13B1D3
        447804DCF86615E20694E286543C9DBD9D50EB8E0020EEDF20612F5F61DC5B1B
        AD3A527F6617326AAD70893B9ECB95559388FB31F15A558D7D5B5553208CE23E
        10CFBC81D068172B61F2479BB347DF53CC3778325A5DDD7166E74EB4837880E4
        BE9438B51CEDC3E0E446207FCFA26A46A82549094BB2F4A1075EFDDD5CF9F89B
        FDCFAC0FD51A75E7B6F35D436C621083643299DA7FC34AD822E7CE04371842EA
        C45CF4D98491ED7CE158197CF731002EE2028013BE4FA8D300C001F64DFAC13B
        F2274C79BCEEF48ECFEA513BD40B6B1F2ED6503A5DFD986569C22A5C735CD723
        DF2444F2B46E781071EA37B8F58E933FDA1CE3F2985FF46755354AC43F120F87
        11F39D4CBAE045A1EBA69AE7CF0470703B84A791E7FB094C8853988AA10DEE3B
        F62953CF3EE16FD30500EF7888DDDBB95C40CFE52A7F6E3A0000B11D8267F347
        07CB36B1F9D673593631233A988E471E797A1D1108A5018088FF90BBC822EFFF
        066F2F79A6E6E2667430DC3E88FB18193904809AF7D1BBD5780F9FED0E042218
        F70E336FD5AF0880813B2D976A733AFD2B74FF9FF57F776DF5EAC3DDCDCD78D1
        1A567C81276C0088BEE44C4CC57683601C207963B5FFD123FFCA14EB87FDBF0F
        0ED5E56ACE6FED213DC56D919BD96CF972C308FDE2E400B06CCE76889E75896E
        6F6DC0898D2AEE31286E3F97F41DF96748BF042BDCC76880797997260740F301
        5B99C37F7F204FD49C324686DF6795740046179658818003B95C90D05D5130C2
        A52889239A368E1B41E701895CFCF3E958913B118700F07EC9D9A8B130F85256
        D677E281078EB9797D34B7ED2EA16D3ED9032F376BA9DBC92B929929FD0DC101
        41B172C950320C0123D90C06466A9152FE3A19813CBB49672653B59F3C000E66
        0B0632557A8D8C4684AF24712E39C4B6F886169CBB8BED3728123CC926078278
        E4F1119E389B6F62551C00C4FD3F465F201B76FEFB5077E35355D7D6417CF0CE
        879EB949E29FAC7FED71EF6AE4FD1D23F19F05175B2EEA44F70F5C91984CFFB7
        39933B21FAABC5F339594E9EDFBAF5F9D1DA5A88FD0A5698D28D8B7EBEBC7B56
        13356E2406A1FE585D767B59B39C53D71B5A6607117C9D92D657334B5E291B4A
        AFF0BE97D3E92A6224A58A74F66D454991B1965B46445FCD9C09360D2ED14551
        9E16085C2A91EFF49B62BF03B8721300B0E60043B01C9DBA969F93B3EAD09257
        1EFA0259B6D0C795EEC5B7481B75A7D3B5BF43BFC86FB840C81E0804469E7789
        8417113B03EDF64F3647BECDB9E65FE8A1262CBBBAD1D4F4D2F5B56B81DE3AF7
        14244CA7D06E970BB6D85C2CC726D550E5F6D13AE1888A3E82BEBDE63E1F2A80
        997389BDDEFDFD1AAAAB9907E6C9B87A3222DF89F8FE1157712F42AE52EC49A8
        13004096FFFB48EFFCB5782EDCB9E6CB153757F18D12C1D9E0B26E12FF26897F
        CC14CEEB36594E9DD1F5185E1E06609479229A2FFD1A2E4620379D1B5CCE0949
        9862E5E5374EEDDE7DCA747C7EA81531B3C715B75D3E93674E1234120040B846
        B736B9B5D1ED23D8203D2EE17B5C40A0D40ABF5DE98225C0A6E6EE52A480B825
        0E27B6E112DB9A4C22160080441B326BE0A197E4099AD67B961EDDFF77A4B9E0
        F68599277AAF9268DB6F59FA2F886D685AF45B8A9236DCB6D111A2F807188A66
        CE22EEFFA0ABF70B9E29A7AA48EA7428EEA474C3FD336E87A25D9E92ED2613B2
        7ADD2D51316FFFD8B183A502A0DC25F652B72E718F505D0022A4D16DE6E535B4
        338C30C7805EE25E0B69A69440F8C9B89C6F3B2B72B7E512BCA4F728E86C326E
        FE039DF953E194557569E3E743B796A1519EC66DC425E49554AA8EE8260BF3BF
        4DACFCFD2EE9B772E62DC512A37F50071372E751230F53C3DF628E7AF16E4E0D
        5E5BB3E6859BAB5671AB9FB1C2980217FD765E1FE666F59A06C1411088E2F56E
        E5BB7C03C0D799B721543F01A3C06B2100045D62D7F88E0029DF598C6F1EC907
        BDC2EEEF6A5C3054306FD7D13B897991E0454579C92F3D1900C8F2D788FDAF93
        F8CF8773E564F0EAD263FBFE8539411F1035E112B583C47F8AC43F3836BFAF0E
        59B59D640062856EC8FD6D97407C9E2EA520FAD7E6ECEC0DBDDFE423BE35585F
        7F062B7BC93C06F834B723459792EFD6C153C94CD9114474701C6216905CB0E2
        37BAFDC077FB169922EAB46FC1A6F90ED9374708E011E68597CBDC7E0909C7A0
        4B2CBE4F30DFBB40117E5FEEFE2EE082CD0F00519473825B6C9ADC3D2D00AC27
        002819F51D9662FE3E534CDBFDAB69DFF6E9E0701D1EB29E15AEE7BF4CAECD2E
        726DDAC4C6485A3FEF66FE40674252885BBEF3A55F79114D7ABF991A3D4C0FD1
        EC7FB05838DC8554AE86A2F09C3EE890DBAC309B670FF344FF94861F111F56F7
        7FA2FA24F3650D2FA18C63881553B108048798B7F55CB188624EA83C82C3673C
        E9CC0B30A1FFC5B90BFEBD84677DB550810A20B11636B54C63A4A5E3BD869E79
        53C3E9D6A3CC31542A9C0EF056D890F8FF1FC4F3C2B4682BADEB91E7C8C5D1DD
        17F00FFEDC6442F48F9003CFE1EB5291D5461927DAF77CC289F655B8EDC1EA17
        1339C308E4FBF54D29FA89F83F49078C6836B3D757E20482CFAB6AE23364EB0C
        328F5345DBA318D1BCA96D85851B6AF3BF340C00608EB86F762B8804EEB7A37E
        CCE13888FF682E17FE47BABC2EDF9094EB23CEE0D13F9E7DD39F34D1D6A36D8E
        087E8A6EBEDDFF40C8E87169E3C617861A1AD061D5EE338A19BD78B4AFDF3D7F
        47D14F8487EEC566D59F769F6D264A8E40F0354D8BFDA92C67ED35866C116E1A
        89520C00E0F846F7C8B91F9DE8EA43D6954E576FB12CEDF3E2B58A327E5CD312
        10D19000D093E2E00CCFFC61BAA9DB0FFAD7F2A36094EF724BCBF76E2D5F8E4E
        848806080026188F3C873FCF1E6A278D9E4AF4130048D75BC4F9D264229FC4B4
        9926C335E20CA76245913D67B01C954D3E6782BCD2F4C7753D7A9079A1D592F6
        2E5848A51800E0CA40EF82F3C1E17C872EBE4F4F3F89FF3F244CBC47EC0C4D8B
        7C5B51F273FF455DCD277FC41F3F7C189B347E9E79C19C7C01F16F3435BDD2B5
        660D0FF6405FA684B6407CA80048A161F779EE18EE25E26F87DEA6D7AC2EF235
        42B2FD8A926C27514EF7B4CA24C9027883CE7A0449338C402DD57ACB52C3FE49
        A3D8E88A0CDE4FD291AF34E61B47C3069A5656D3F92C7E0000F1E0149E461DDC
        0F2E34DD178B18869ECA662BB1E7DF1AA13386757DF43075208F13804879F1DF
        D2D131BAB2AFEFB7E9F37F61AC30BF1E8A490DDC6C6C04F12344159ED001BA15
        3A962FECEC74DBE5D9433353887E0098D4147B7CE2B756963C96764D8B7712E7
        F3A4D1B8A7CABC99C536179314D00D23589BCB952DF126869A2912FF2F2A4A06
        0B34BA98A3EE2095005680F375C523E6B2F801804E0831CFADE1133C2C9720E3
        24FEB793F87F56BC8E44E1251285E0509E8133BF167FCBF9F372C3D0D047E846
        6F2BF6006043847989F8B88EDB1BB015780A771EE9E39B45D8539AA6EA6002C0
        7FA6C31FB1092B71AD2C71FDCBA4AE002271134A93150EA97277D5DEEC92805F
        9FCB95AF266910246FA797BC9D9BCC9BE388E7EB129E31BE5801C0FD618D795B
        B3F2620F4992F8FF1891FCB7C5EBC8683FA4AA69B8311093E0D08EF278BC6FD3
        A54BADE589C4CFCA5E10A7A018849CEEA6A6E33756AF065121F60140101FA214
        563E273ECF1500DB624AEE22E2438D61D28ADFE227BD9D6AD7F518B815600360
        016E109D0F9BF22DE03900B8BF5F699AEA325209CDAA3A3E46D28EABA72EE625
        B8E6A3918B1300BCB84098B09A269B0D935E0CBE226EF94A7D9A20F17F48964D
        7B66AE96CD0EAEBA7123B4ECD6AD87FDB3780BDA52D558E7DAB5AFDC5AB60C83
        E0E044DDED741081E7EFE79C7FCB3D6F13666A97EFDDBF4ACFF829E6E37E49CA
        92DE8E5E26B1CFB7998701095B02FA1BC40478FD00401B908870496B2D8B3548
        523E628967E2BB987149B0F80170878E45A208C406F273DBA8536F06F5D19381
        54AA76455F5FCD92818115814CA6F64E0D8F9795F55FDCBCF978ACA202448074
        009771CEB75D4DE6ED11C489CF770A99E219DF032982ACDDFEF97A2661EE5555
        B5571443EC83F87CA10AEE034F430440967911425B02304765F0F910AAFB4C00
        0FDFD7884BAABB4E3E31D7254FA703EE40CF473DE417E9DC273F4C82E113CEBC
        058B952763AC21DAD3BD24D2A757C4E3F5FE255B458A85F97C44FC8B195D178D
        4C3EA79F4FECE4D63EDF23A8E46D620800D8DA0D91BAC2FC3D526E10BB9393E8
        E609AA4168DBAD65850666A900D0DCE7E63B96F3695750530B7A8F80827EE11F
        0800DFB41CBD0FC3EB067DD167798310F85DD9F5652D30E636869351569E8AB1
        4036CD14B3B4A17712F9F19B4D4DA7C8DA8F5AB23D7DACCC6D17ED83C87C6730
        DCBF974DDC17B0A40E2529F5216AF633CC27DD1465FC32197EB8170F6AE11E20
        3C9FB9D3CB3CB0F15C3D5C15E21AA8018007B60AC00000E498B7BF219F769558
        C8E9E1FDC5EE2444E6ECD9BFC2C08E5BFC8323D32EE4E2E52255555D9DEBD65D
        4984C341B7F3A0422022B90805F141F82EE64D5CE43EF5B4A26B0480CFD323FF
        72E1598B04CED82559CE8290E05A7F82EA2EF79EFC7E79F12D4C1CE5C660857B
        E42387DC7B880BD7CF1EC566B8D8C425EEFF201D3E37930D63342F5E5EDED7DD
        DC7C79A8AE2E475CCF0D2F741C8800AB1B5CCE1773705F9A8BE192C5BE580800
        67E8B50A327993A71927F1DF4DC61F1FA412E728DA194A99378BB8E8FD5CC318
        EA8A7B479C31F8285D864D43522D94C201F0753AFCF84C34481C6F462B2A467B
        57AEBC365C57173515850F93F205A2D0AF2002F425F42E0CA75EE6ED000E42DC
        555CDD75FF40D0827905C4F9A3048001F7FEE270329FA5646FF89C4A35E07E18
        0E8F1E3B7630EA6FDF950693E51A98D6BAFC8552A4038E41C3336ADE55B11DE9
        60D018A9A919BFBD74E968ACB2326139AB16B811053109A2A253C1E12076AF5B
        218E41802977009FAA10F73F41B77C8609F90B501425D9A769717E7F3E4389E7
        EEB187A9991DE368808AFA1B7AA347E8881C3458D984012E028CD571ECD83F2D
        C874AFAFA70000980983D465089AC0CABDA3DEB717BAD22FD29A6EA4CB02D0EF
        C6686DAD44E2DEC8691A9FBC80239FD8804E03614160719992183AE5B3666CE3
        EB6E752801E0BDF4E85FF13F3FD99F3DAA9AE4BB83FA7314E777F6220902E314
        6B21F82CA7BC68A78F78CE73CC49498F29F31708E3D78E1EFDFAA270F7262B5C
        05E0C5EB2E6CDAF491603ADDA8A7D3D5E4EA952B865167288A9C3303951935A4
        65559D25F5104B06CB9915CA8C9A010B93D9B94E34DCCEE2F3D0F9FC736E1D8B
        EBD46E336FF004BF815A78DDA3680400B26524BF2D636A5AB45F51D2FE194ABC
        E687A9E9FA46E412644E9C60B2623AEF695D90E5CC7B753D0AFB459CA235A333
        7666BBF843C1C8618710297C68A88666CB922A32999A06244510FAC00A042249
        2C4A64DE02069EFB07E294E7D8E3F3E2440070C27343CFCE8A3913FA9308F83E
        7AA52FFADF4BD3C6FA14258367F1EF4F006930C48147D7BF952E3D54DADDCC93
        44FCDF24FB82AF4114A760CFDAA294992E7E00FC3E73086F2784A2BAD234B525
        994CF566F177F4D20652BFB92FCB892E129A275814CFF3A40D7CDCDC36F4D80C
        66C12402FE06BDD25FFACFAB6A6C4055537C7368FF762EF945AA2E80B0E9C494
        6BF89070211018FD33E64DFCE4C09FD60EA7F35DFC00F81073880F770D52A021
        9309EF37CDB282C44FA45363A453F90446AEDBEDB902CC11EF7E00F0CEE199C1
        ECDC3D33DD49AE0D40465C210115653CA26909D1F5130110E700DCB7EFC96649
        B27E823E3E6459B003A4E5EE9AC7098090E5D4615D8FC17BE24BAE51F9BABBC5
        1709442100E0E5417CDD3DD69039F05ECB52373999B4502C8BB8BF4F9673D07B
        403B880E838EC7C2F9DABC28F382231C2CD34E63369D42047C9848036A1684A4
        65399D203BE022D9AFFEEC9DC8E33F2EBC3FAE832BB98654DE56C3D01EC8E5CA
        7F8A5AA8F2DF8B98E01831C151E62DFEE8635E7692C4620500322D82F87CF66A
        3975C27202C016D3547751A7AC23000448F7F513A7F0ADDE6104C19502875DE5
        1DC00A896E4F6B9EED4ED9B7EF5D1B89DF7F40AF5598C69D990689EB764932FD
        79FC8B0100B1FED5A886A1EFCB662B3E4000F01985161996635715258B8D213A
        DD77EF62DE9C80BBDAF7683E8A1F00E838714D3C4080208E3D51940C420280B4
        8E3A12811688453E4D9C4FD902002006797AB25927BA58DEF4A6B79767B3E52F
        D2A3F9661A1384D5C40DE258F8F493AA006C08C51C23D84E899BCB85DE46F537
        C4C45776A791F14B0088C8B2211A955798378A195BAC00F0E7D2C3DF088E6072
        286608AF762BCFCB03E30762BFCB7D714883E87CBD3CB6744BA76BFF9824D587
        27BCA8944BE8FAD8F709BC08EE7029C08D40D3BD9ECF440608C8FEA9FC45D30C
        FC7291B64C6A2B4E6DF9BD0A1E595C9C1260B24E65CE2451800061524CF2A875
        AF859EE7411DE841E8C0E47CB940D8DB3793A9FA09D3D4B155FD84844C64B875
        6A5AFC19525F10DD793790B903382E0020EE31E0539D4ED71C20D9F15313DB49
        E7342D865941FEA8E282DE20AA5829150076A088394058C2BC40090C3C3119D1
        F07C6E94ECEAF0E5A954DDDF13991E29F2134B519264BC8D7F83B897EFEA01D0
        DAD3CCDC011F7B66B093F6B6F2CBD4CE63FE46A88D340109F68FB8E9653EB278
        375BDFCD57290500300841F04AB74244F2A9E23CCCCB77128BCDE74C185785D5
        6532153F639AC14F3061E69257AC9C2C67CF284AEA6BB29C7981B8F8BAFBEC3C
        73B99D0286B8BF91B81FA9F0266C7E441E4082EC09B87B6258998360C16E1259
        AC940200BEA60D44876B08A39067FFC08BC2E0E38B21E7753E3CDFD7808CD5D5
        994CF59F1001DF3EC93B62E1F120E9F2671525F3B744CC33A43692B95C206359
        0193AE27896791E8973EC626AC21B42C12FF317759981F007C9BD8C53521648A
        4EE5BFE3A9D514DF757C52446EBE5FDC7D568074B9EBC27D9C1EBBF10E97B85B
        C858EEA410092A0D126E23BC1D56C48E804B4906604C9673B07DFC06200030BC
        580C409469CFF611B266F9CB821804715516EC95E65C2E486E5C3952D8D4CF54
        FBE4022207520C6B5958F1C8E2B437BF9CCF32D7C98B67BDB87600AC78C434D6
        92587F9C40F00B24B896BDCEA6ED2249D92C01001E005C5EFFC650DC059CEF6E
        28FD7DE6FB0166BAF0CD2D98A3BBE1B6AE354D753781E027C9B25FE385B4EFAE
        900B98D6F5288C46C43DFC00E85A4C2E20CA3D070014D770B507B39813B7584D
        86DD2602C05EC328DB4980A8F26F00515AB1238A69321AE1FAF21C4522006E2E
        D45DC2272BF72A00B83F8F411CC42D10DA5D4D759D65C91B09002415F4E5D8E3
        C0D9DF08D39CB0FA170B1E24DE2FEE163496214926B623CE91C76039797AADC9
        3C80FEC5E402F217BD278B6B0BC023000820090002E4215ACD9CB0366C823A02
        413581A20C5BC4203D2BFDAD3A9B2A2039B369DACB0965E453B4E8BCED0181C0
        887A72EEE7790BA0FF8716CB8A205EEE6500E0C057F5F0051D18EA5DC9BC1C7D
        00066C05188D240D5880882CE6F0E1F300B8AB8B28275C45BE16109567130328
        228BC90340B9670180E20301888C90B698DBAF817929DBC2EEEF6040F2CCE0BC
        7F78E226440BF9AA223B1134F3E602F035816F006021150104202C0F69F3B18D
        7AF7334F46C5771CE7FB178AAB7F440060F693B8FB199FE73827594A67B2DCF3
        00407141C0E73880C07CC40F9C8FC81F00204A009D15463C79BA763E0732BF19
        0473C611EC798E8BCD0064EC3E01002F42DE03FFD80688CFE7421653011C0076
        9614E64D7513A7BB2D9A89A062B9AF00C08B0B04BE6A49CC88A20B753209C0B3
        7816A46D5D8CC467EC3E0580587C835D5291CF287C01687EF107AF8B4DE7FBCB
        7D0F80FBBDBC0180FBBCBC0180FBBCBC0180FBBCBC0180FBBCFC7F0BE99006B3
        DDC8540000000049454E44AE426082}
      Transparent = True
      OnDblClick = ProgramIconDblClick
      IsControl = True
    end
    object LinkLabel3: TLinkLabel
      Left = 8
      Top = 202
      Width = 280
      Height = 41
      Anchors = [akLeft, akBottom]
      AutoSize = False
      Caption = '<Language> translation by <Translator>'
      TabOrder = 3
      Visible = False
      OnLinkClick = LinkLabel1LinkClick
      ExplicitTop = 272
    end
    object Animate1: TAnimate
      Left = 8
      Top = 8
      Width = 0
      Height = 0
      Visible = False
    end
    object LinkLabel1: TLinkLabel
      Left = 8
      Top = 219
      Width = 280
      Height = 27
      AutoSize = False
      Caption = 
        'The newest version as well as the source code are available at: ' +
        '%s'
      TabOrder = 1
      OnLinkClick = LinkLabel1LinkClick
    end
    object LinkLabel2: TLinkLabel
      Left = 8
      Top = 196
      Width = 48
      Height = 17
      Caption = 'Email: %s'
      TabOrder = 2
      OnLinkClick = LinkLabel2LinkClick
    end
  end
end
