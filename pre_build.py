Import("env")
import os
import shutil

# Chemin vers la lib TFT_eSPI dans .pio
lib_path = os.path.join(env.subst("$PROJECT_LIBDEPS_DIR"), env.subst("$PIOENV"), "TFT_eSPI")
user_setup_src = os.path.join(env.subst("$PROJECT_DIR"), "User_Setup.h")
user_setup_dst = os.path.join(lib_path, "User_Setup.h")

# Copier notre User_Setup.h dans la lib si elle existe
if os.path.exists(user_setup_src) and os.path.exists(lib_path):
    print(f"Copying User_Setup.h to {lib_path}")
    shutil.copy(user_setup_src, user_setup_dst)
