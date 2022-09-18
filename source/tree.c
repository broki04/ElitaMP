#include source/version.c

// ***** CORE
#include source/core/def.c
#include source/core/hex.c
#include source/core/var.c
#include source/core/design.c
#include source/core/zone.c

// ***** UTILS
#include source/utils/records.c
#include source/utils/log.c
#include source/utils/level.c
#include source/utils/ulepszenia.c
#include source/utils/achievement.c
#include source/utils/quest.c
#include source/utils/quiz.c
#include source/utils/admin_gifs.c
#include source/utils/hud.c
#include source/utils/admin_top.c
#include source/utils/duel.c
#include source/utils/premium.c
#include source/utils/ranks.c
#include source/utils/mentioned.c
#include source/utils/areny.c
#include source/utils/glitch.c
#include source/utils/interiors.c
#include source/utils/bank.c
#include source/utils/heist.c
#include source/utils/pedofil.c
#include source/utils/spec.c
#include source/utils/report.c
#include source/utils/top.c
#include source/utils/chat.c
#include source/utils/driving_school.c
#include source/utils/radio.c
#include source/utils/anim.c
#include source/utils/nodm.c
#include source/utils/vehicles.c
#include source/utils/truck.c
#include source/utils/afk.c
#include source/utils/chicken.c
#include source/utils/tutorial.c

// ***** ATTRACTIONS

// teleport system
#include source/teleport/tp-main.c
#include source/teleport/tp-list.c
#include source/teleport/tp-creator.c
#include source/teleport/tp-manage.c

// privcar system
#include source/privcars/pv_init.c
#include source/privcars/pv_editor.c
#include source/privcars/pv_fuel.c
#include source/privcars/pv_manage.c
#include source/privcars/pv_ulepszenia.c
#include source/privcars/pv_attachment.c
#include source/privcars/pv_keys.c
#include source/privcars/pv_dialog.c
#include source/privcars/pv_buy.c
#include source/privcars/pv_cmd.c
#include source/privcars/pv_timer.c

#include source/privcars/gielda/gielda_core.c
#include source/privcars/gielda/gielda_dialog.c
#include source/privcars/gielda/gielda_buy.c

// team system
#include source/teams/g-core.c
#include source/teams/g-ach.c
#include source/teams/g-loader.c
#include source/teams/g-zone.c
#include source/teams/g-info.c
#include source/teams/g-base.c
#include source/teams/g-base_obj.c
#include source/teams/g-heist.c
#include source/teams/g-creator.c
#include source/teams/g-members.c
#include source/teams/g-market.c
#include source/teams/g-level.c
#include source/teams/g-spar.c
#include source/teams/g-ranked.c
#include source/teams/g-dialog.c
#include source/teams/g-cmd.c

// job system
#include source/jobs/job_core.c
#include source/jobs/job_level.c

#include source/jobs/list/magazynier.c
#include source/jobs/list/grabarz.c
#include source/jobs/list/zrzut.c
#include source/jobs/list/taxi.c
#include source/jobs/list/mysliwy.c
#include source/jobs/list/drwal.c
#include source/jobs/list/grzybiarz.c

#include source/jobs/job_func.c
#include source/jobs/job_dialog.c

// business system
#include source/business/b_core.c
#include source/business/b_creator.c
#include source/business/b_panel.c

#include source/business/types/casino.c
#include source/business/types/gunshop.c

#include source/business/b_loader.c
#include source/business/b_money.c

// fishing system
#include source/fishing/fish_core.c
#include source/fishing/fish_ulep.c
#include source/fishing/fish_shop.c
#include source/fishing/fish_save.c
#include source/fishing/fish_main.c

// mining system
#include source/mining/mine_core.c
#include source/mining/mine_main.c
#include source/mining/mine_menu.c
#include source/mining/mine_sell.c

// games system
#include source/games/game-core.c

#include source/games/creator/rc-c.c
#include source/games/creator/wg-c.c

#include source/games/game/ch.c
#include source/games/game/zp.c
#include source/games/game/da.c
#include source/games/game/rc.c
#include source/games/game/btr.c
#include source/games/game/sm.c
#include source/games/game/gg.c
#include source/games/game/wp.c
#include source/games/game/sk.c
#include source/games/game/wg.c

#include source/games/game-objects.c
#include source/games/game-draw.c

// event system
#include source/event/event_core.c
#include source/event/event_cmd.c
#include source/event/event_panel.c
#include source/event/event_settings.c

// ***** UTILS
#include source/utils/ac.c
#include source/utils/bet.c
#include source/utils/punish.c
#include source/utils/settings.c
#include source/utils/disk.c
#include source/utils/figure.c
#include source/utils/loader.c
#include source/utils/account.c
#include source/utils/aduty.c
#include source/utils/stats.c
#include source/utils/banlist.c
#include source/utils/apanel.c

// ***** CMD
#include source/cmd/player.c
#include source/cmd/vehicle.c
#include source/cmd/mod.c
#include source/cmd/admin.c
#include source/cmd/vice.c
#include source/cmd/head.c

// ***** /HELP
#include source/help/help-core.c
#include source/help/help-cmd.c
#include source/help/help-admin.c

// ***** CORE
#include source/core/func.c
#include source/core/data.c
#include source/core/class.c
#include source/core/dialog.c
#include source/core/timer.c
#include source/core/init.c
#include source/utils/vault.c
#include source/cmd/exit.c

// ***** OBJECTS
#include source/objects.obj