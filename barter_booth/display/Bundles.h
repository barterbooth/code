#ifndef __BUNDLES_H__
#define __BUNDLES_H__

//Security
#define MAX_LANGUAGES 64
#define MAX_KEYS 256

//Languages
#define BUNDLDE_FRENCH 0
#define BUNDLDE_ENGLISH 1
#define BUNDLE_SPANISH 2

class Bundles {
  public:
    Bundles() {
      _language = 0;
      _size = sizeof(_keys)/sizeof(String);
      _languages = sizeof(_values)/sizeof(String[])

      for (uint i=0; i<LANGUAGES; i++) {
        uint16_t valuesSize = sizeof(_values[i])/sizeof(String)
        if (keysSize > valuesSize) {
          for (uint key=valuesSize; key<keysSize-valuesSize; key++) _values[i][key] = "??"+_keys[key]+"??";
        }
      }
    }

    String get(const String key) {
      if (_index < _languages) {
        for (_i=0; i<_size; i++) {
          if (_keys[_i].equals(key)) return _values[_index][_i];
        }
      }
      return "??"+key+"??";
    }

    void setLanguage(uint8_t index) {
      _index = index;
    }

  private:
    uint8_t _index, _languages;
    uint16_t _size, _i;


    String _keys[MAX_KEYS] = {
        "finger.image.found",
        "finger.image.notfound",
        "finger.error.fatal",
        "finger.error.imagemess",
        "finger.error.missmach",
        "finger.scan.inprogress",
        "finger.scan.finished",
        "finger.scan.usercreated"
    }

    String _values[MAX_LANGUAGES][MAX_KEYS] = {
      { //FRENCH
        "Empreinte digital trouvée! \nRetirez votre doigt ...",
        "Aucune empreinte digitale trouvée ! \nVeuillez réessayer ...",
        "Une erreur fatal s'est produite ! \nVeuillez réessayer ou contacter un administrateur.",
        "L'empreinte digitale n'est pas lisible ! \nNettoyez votre doigt ou choisissez-en un autre puis réessayez.",
        "Les empreintes ne correspondent pas ! \nVeuillez réessayer ...",
        "Analyse ...",
        "Analyse terminée !",
        "Utilisateur créé ! Vous pouvez retirer votre doigt."""
      },
      { //ENGLISH
        "",
      },
      { //SPANISH
        "",
      }
    }
}

#endif //__BUNDLES_H__