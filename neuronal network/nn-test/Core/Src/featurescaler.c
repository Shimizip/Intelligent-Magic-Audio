/*
 * featurescaler.c
 *
 *  Created on: May 16, 2024
 *      Author: Leon Braungardt
 */


#include "featurescaler.h"

/* Feature Scaling vectors 30x32 flattened to be used for Model 93 */

const float featureScalerMean[960] = {
  -7.7597937584e+00, -7.7420225143e+00, -7.7399463654e+00, -7.7228636742e+00,
  -7.7230215073e+00, -7.7589945793e+00, -7.7449016571e+00, -7.7711572647e+00,
  -7.7489819527e+00, -7.7222084999e+00, -7.7013721466e+00, -7.6851363182e+00,
  -7.6987524033e+00, -7.7429141998e+00, -7.7360796928e+00, -7.7610259056e+00,
  -7.7568540573e+00, -7.7390956879e+00, -7.7286958694e+00, -7.7588939667e+00,
  -7.7547588348e+00, -7.7502217293e+00, -7.7680068016e+00, -7.7633638382e+00,
  -7.7462568283e+00, -7.7829403877e+00, -7.7521896362e+00, -7.7522139549e+00,
  -7.7596812248e+00, -7.7520961761e+00, -7.7297449112e+00, -7.7389922142e+00,
  -1.4611957550e+01, -1.4614995003e+01, -1.4595205307e+01, -1.4593064308e+01,
  -1.4604123116e+01, -1.4617896080e+01, -1.4610317230e+01, -1.4621142387e+01,
  -1.4608151436e+01, -1.4579716682e+01, -1.4609406471e+01, -1.4608696938e+01,
  -1.4604053497e+01, -1.4610126495e+01, -1.4615518570e+01, -1.4618196487e+01,
  -1.4631410599e+01, -1.4644005775e+01, -1.4618592262e+01, -1.4626495361e+01,
  -1.4621869087e+01, -1.4625506401e+01, -1.4645422935e+01, -1.4640667915e+01,
  -1.4646067619e+01, -1.4655504227e+01, -1.4618909836e+01, -1.4622437477e+01,
  -1.4610999107e+01, -1.4619186401e+01, -1.4600025177e+01, -1.4598844528e+01,
  -1.8194787979e+01, -1.8204034805e+01, -1.8198862076e+01, -1.8173810959e+01,
  -1.8168781281e+01, -1.8181512833e+01, -1.8186893463e+01, -1.8197744370e+01,
  -1.8182350159e+01, -1.8179882050e+01, -1.8223537445e+01, -1.8201154709e+01,
  -1.8217208862e+01, -1.8222206116e+01, -1.8213207245e+01, -1.8223768234e+01,
  -1.8258678436e+01, -1.8224237442e+01, -1.8240922928e+01, -1.8223735809e+01,
  -1.8236288071e+01, -1.8231698990e+01, -1.8247619629e+01, -1.8240896225e+01,
  -1.8234802246e+01, -1.8243110657e+01, -1.8254024506e+01, -1.8210151672e+01,
  -1.8219593048e+01, -1.8181089401e+01, -1.8191719055e+01, -1.8182245255e+01,
  -2.0865642548e+01, -2.0886371613e+01, -2.0859050751e+01, -2.0848266602e+01,
  -2.0844507217e+01, -2.0867336273e+01, -2.0860767365e+01, -2.0848243713e+01,
  -2.0877204895e+01, -2.0865217209e+01, -2.0866550446e+01, -2.0864719391e+01,
  -2.0862876892e+01, -2.0852939606e+01, -2.0875276566e+01, -2.0880569458e+01,
  -2.0873601913e+01, -2.0852182388e+01, -2.0879056931e+01, -2.0883462906e+01,
  -2.0889513016e+01, -2.0886754990e+01, -2.0896720886e+01, -2.0865636826e+01,
  -2.0887552261e+01, -2.0904333115e+01, -2.0889545441e+01, -2.0856729507e+01,
  -2.0846931458e+01, -2.0837434769e+01, -2.0848072052e+01, -2.0844434738e+01,
  -2.2467189789e+01, -2.2480834961e+01, -2.2457168579e+01, -2.2478963852e+01,
  -2.2454389572e+01, -2.2481346130e+01, -2.2493562698e+01, -2.2478136063e+01,
  -2.2483394623e+01, -2.2476638794e+01, -2.2457916260e+01, -2.2483230591e+01,
  -2.2460643768e+01, -2.2482597351e+01, -2.2502006531e+01, -2.2481609344e+01,
  -2.2492353439e+01, -2.2471311569e+01, -2.2484510422e+01, -2.2484863281e+01,
  -2.2499898911e+01, -2.2482824326e+01, -2.2513563156e+01, -2.2514001846e+01,
  -2.2528680801e+01, -2.2513223648e+01, -2.2488082886e+01, -2.2476648331e+01,
  -2.2480693817e+01, -2.2466056824e+01, -2.2453815460e+01, -2.2443765640e+01,
  -2.4373695374e+01, -2.4350820541e+01, -2.4320634842e+01, -2.4358131409e+01,
  -2.4329786301e+01, -2.4353406906e+01, -2.4363876343e+01, -2.4349512100e+01,
  -2.4352535248e+01, -2.4332489014e+01, -2.4361839294e+01, -2.4352310181e+01,
  -2.4350738525e+01, -2.4360275269e+01, -2.4358133316e+01, -2.4358575821e+01,
  -2.4360645294e+01, -2.4344770432e+01, -2.4380702972e+01, -2.4378313065e+01,
  -2.4377742767e+01, -2.4343044281e+01, -2.4374294281e+01, -2.4367254257e+01,
  -2.4389303207e+01, -2.4367645264e+01, -2.4373636246e+01, -2.4381237030e+01,
  -2.4343437195e+01, -2.4321208954e+01, -2.4335639954e+01, -2.4329179764e+01,
  -2.6394079208e+01, -2.6392862320e+01, -2.6412050247e+01, -2.6408016205e+01,
  -2.6401512146e+01, -2.6390731812e+01, -2.6388813019e+01, -2.6393909454e+01,
  -2.6387573242e+01, -2.6375415802e+01, -2.6410568237e+01, -2.6414987564e+01,
  -2.6426385880e+01, -2.6430011749e+01, -2.6419614792e+01, -2.6429605484e+01,
  -2.6423917770e+01, -2.6411579132e+01, -2.6419540405e+01, -2.6407424927e+01,
  -2.6409599304e+01, -2.6388540268e+01, -2.6417438507e+01, -2.6424598694e+01,
  -2.6408580780e+01, -2.6409700394e+01, -2.6423152924e+01, -2.6428312302e+01,
  -2.6371959686e+01, -2.6382732391e+01, -2.6380645752e+01, -2.6386997223e+01,
  -2.8306520462e+01, -2.8286252975e+01, -2.8300992966e+01, -2.8302486420e+01,
  -2.8283226013e+01, -2.8316925049e+01, -2.8303888321e+01, -2.8329298019e+01,
  -2.8309228897e+01, -2.8293029785e+01, -2.8312290192e+01, -2.8340457916e+01,
  -2.8371887207e+01, -2.8333625793e+01, -2.8327375412e+01, -2.8326442719e+01,
  -2.8339967728e+01, -2.8317680359e+01, -2.8341329575e+01, -2.8314548492e+01,
  -2.8320692062e+01, -2.8329271317e+01, -2.8337570190e+01, -2.8331262589e+01,
  -2.8330070496e+01, -2.8315130234e+01, -2.8321781158e+01, -2.8306304932e+01,
  -2.8270257950e+01, -2.8290901184e+01, -2.8281742096e+01, -2.8289676666e+01,
  -2.9616699219e+01, -2.9613199234e+01, -2.9615550995e+01, -2.9594997406e+01,
  -2.9617153168e+01, -2.9636152267e+01, -2.9634273529e+01, -2.9631948471e+01,
  -2.9613712311e+01, -2.9604288101e+01, -2.9634654999e+01, -2.9641145706e+01,
  -2.9644763947e+01, -2.9640439987e+01, -2.9615032196e+01, -2.9629144669e+01,
  -2.9638496399e+01, -2.9616811752e+01, -2.9635442734e+01, -2.9637557983e+01,
  -2.9632692337e+01, -2.9645729065e+01, -2.9631052017e+01, -2.9635957718e+01,
  -2.9649076462e+01, -2.9638172150e+01, -2.9624128342e+01, -2.9617450714e+01,
  -2.9587753296e+01, -2.9569013596e+01, -2.9593643188e+01, -2.9603124619e+01,
  -3.0691173553e+01, -3.0719963074e+01, -3.0708759308e+01, -3.0706806183e+01,
  -3.0709278107e+01, -3.0736705780e+01, -3.0734716415e+01, -3.0732864380e+01,
  -3.0714282990e+01, -3.0699167252e+01, -3.0704015732e+01, -3.0712366104e+01,
  -3.0711183548e+01, -3.0703660965e+01, -3.0684181213e+01, -3.0720558167e+01,
  -3.0705907822e+01, -3.0711025238e+01, -3.0710165024e+01, -3.0720252991e+01,
  -3.0727388382e+01, -3.0713277817e+01, -3.0727203369e+01, -3.0745100021e+01,
  -3.0763519287e+01, -3.0727287292e+01, -3.0732795715e+01, -3.0719486237e+01,
  -3.0702302933e+01, -3.0675821304e+01, -3.0672092438e+01, -3.0658428192e+01,
  -3.2025489807e+01, -3.2045753479e+01, -3.2038589478e+01, -3.2021995544e+01,
  -3.2033962250e+01, -3.2045627594e+01, -3.2062171936e+01, -3.2049209595e+01,
  -3.2047706604e+01, -3.2052509308e+01, -3.2034187317e+01, -3.2032661438e+01,
  -3.2028465271e+01, -3.2033451080e+01, -3.2013515472e+01, -3.2035797119e+01,
  -3.2023277283e+01, -3.2031002045e+01, -3.2059307098e+01, -3.2063743591e+01,
  -3.2053676605e+01, -3.2036163330e+01, -3.2031677246e+01, -3.2046947479e+01,
  -3.2050758362e+01, -3.2046157837e+01, -3.2050540924e+01, -3.2060413361e+01,
  -3.2024482727e+01, -3.2013900757e+01, -3.2022148132e+01, -3.1998485565e+01,
  -3.3404582977e+01, -3.3403190613e+01, -3.3407455444e+01, -3.3395572662e+01,
  -3.3413761139e+01, -3.3409767151e+01, -3.3418926239e+01, -3.3413707733e+01,
  -3.3403026581e+01, -3.3409980774e+01, -3.3414566040e+01, -3.3424221039e+01,
  -3.3431098938e+01, -3.3427673340e+01, -3.3403232574e+01, -3.3396701813e+01,
  -3.3404357910e+01, -3.3389408112e+01, -3.3421524048e+01, -3.3418201447e+01,
  -3.3412624359e+01, -3.3404445648e+01, -3.3413608551e+01, -3.3405956268e+01,
  -3.3387378693e+01, -3.3409030914e+01, -3.3415466309e+01, -3.3426319122e+01,
  -3.3393558502e+01, -3.3375320435e+01, -3.3382240295e+01, -3.3391689301e+01,
  -3.4515583038e+01, -3.4537342072e+01, -3.4536140442e+01, -3.4543682098e+01,
  -3.4553016663e+01, -3.4541324615e+01, -3.4573822021e+01, -3.4578163147e+01,
  -3.4557838440e+01, -3.4546092987e+01, -3.4552845001e+01, -3.4560714722e+01,
  -3.4588588715e+01, -3.4557563782e+01, -3.4533489227e+01, -3.4564151764e+01,
  -3.4544300079e+01, -3.4522541046e+01, -3.4542366028e+01, -3.4548126221e+01,
  -3.4553710938e+01, -3.4567195892e+01, -3.4559535980e+01, -3.4553173065e+01,
  -3.4530731201e+01, -3.4530872345e+01, -3.4548645020e+01, -3.4557067871e+01,
  -3.4527664185e+01, -3.4512485504e+01, -3.4527183533e+01, -3.4506519318e+01,
  -3.5668388367e+01, -3.5661205292e+01, -3.5666049957e+01, -3.5673721313e+01,
  -3.5671276093e+01, -3.5702312469e+01, -3.5716751099e+01, -3.5718589783e+01,
  -3.5675716400e+01, -3.5674552917e+01, -3.5679874420e+01, -3.5711547852e+01,
  -3.5726402283e+01, -3.5709087372e+01, -3.5677791595e+01, -3.5710796356e+01,
  -3.5694187164e+01, -3.5671836853e+01, -3.5702907562e+01, -3.5695518494e+01,
  -3.5687736511e+01, -3.5685493469e+01, -3.5689685822e+01, -3.5691631317e+01,
  -3.5691116333e+01, -3.5670101166e+01, -3.5692974091e+01, -3.5705821991e+01,
  -3.5673984528e+01, -3.5653347015e+01, -3.5671257019e+01, -3.5652122498e+01,
  -3.6869377136e+01, -3.6862678528e+01, -3.6882572174e+01, -3.6887226105e+01,
  -3.6850433350e+01, -3.6879295349e+01, -3.6911380768e+01, -3.6907382965e+01,
  -3.6916194916e+01, -3.6907920837e+01, -3.6899581909e+01, -3.6922153473e+01,
  -3.6934566498e+01, -3.6926525116e+01, -3.6920459747e+01, -3.6891368866e+01,
  -3.6875255585e+01, -3.6873329163e+01, -3.6903842926e+01, -3.6883674622e+01,
  -3.6874652863e+01, -3.6870990753e+01, -3.6893184662e+01, -3.6884838104e+01,
  -3.6901153564e+01, -3.6870399475e+01, -3.6891769409e+01, -3.6896224976e+01,
  -3.6873329163e+01, -3.6861495972e+01, -3.6861736298e+01, -3.6839756012e+01,
  -3.7855293274e+01, -3.7863235474e+01, -3.7864959717e+01, -3.7869049072e+01,
  -3.7849929810e+01, -3.7887744904e+01, -3.7898979187e+01, -3.7891899109e+01,
  -3.7901088715e+01, -3.7920391083e+01, -3.7907257080e+01, -3.7944770813e+01,
  -3.7942245483e+01, -3.7947723389e+01, -3.7921497345e+01, -3.7936870575e+01,
  -3.7892845154e+01, -3.7871417999e+01, -3.7893260956e+01, -3.7875537872e+01,
  -3.7885208130e+01, -3.7889003754e+01, -3.7905063629e+01, -3.7877307892e+01,
  -3.7896003723e+01, -3.7872051239e+01, -3.7885921478e+01, -3.7885845184e+01,
  -3.7869853973e+01, -3.7842296600e+01, -3.7836490631e+01, -3.7834102631e+01,
  -3.9245956421e+01, -3.9276279449e+01, -3.9262111664e+01, -3.9254070282e+01,
  -3.9271457672e+01, -3.9264648438e+01, -3.9290348053e+01, -3.9307598114e+01,
  -3.9284469604e+01, -3.9311550140e+01, -3.9330116272e+01, -3.9344074249e+01,
  -3.9341365814e+01, -3.9346927643e+01, -3.9320926666e+01, -3.9328712463e+01,
  -3.9307743073e+01, -3.9304134369e+01, -3.9304981232e+01, -3.9288002014e+01,
  -3.9288974762e+01, -3.9295345306e+01, -3.9304794312e+01, -3.9280189514e+01,
  -3.9281795502e+01, -3.9264019012e+01, -3.9287918091e+01, -3.9312591553e+01,
  -3.9265060425e+01, -3.9229022980e+01, -3.9222881317e+01, -3.9225467682e+01,
  -4.0620925903e+01, -4.0646724701e+01, -4.0642314911e+01, -4.0668640137e+01,
  -4.0645061493e+01, -4.0642028809e+01, -4.0656417847e+01, -4.0684818268e+01,
  -4.0651035309e+01, -4.0665172577e+01, -4.0672271729e+01, -4.0688606262e+01,
  -4.0689060211e+01, -4.0683124542e+01, -4.0661857605e+01, -4.0679782867e+01,
  -4.0663490295e+01, -4.0658763885e+01, -4.0661743164e+01, -4.0658927917e+01,
  -4.0648906708e+01, -4.0642639160e+01, -4.0668941498e+01, -4.0654983521e+01,
  -4.0642841339e+01, -4.0640274048e+01, -4.0655769348e+01, -4.0667209625e+01,
  -4.0636940002e+01, -4.0623340607e+01, -4.0623256683e+01, -4.0615665436e+01,
  -4.1726997375e+01, -4.1752693176e+01, -4.1765033722e+01, -4.1768020630e+01,
  -4.1745391846e+01, -4.1751445770e+01, -4.1772190094e+01, -4.1789031982e+01,
  -4.1783378601e+01, -4.1791519165e+01, -4.1784793854e+01, -4.1793270111e+01,
  -4.1803615570e+01, -4.1805530548e+01, -4.1772441864e+01, -4.1810104370e+01,
  -4.1771957397e+01, -4.1739368439e+01, -4.1766944885e+01, -4.1776969910e+01,
  -4.1744316101e+01, -4.1761306763e+01, -4.1770114899e+01, -4.1772041321e+01,
  -4.1757904053e+01, -4.1751998901e+01, -4.1749095917e+01, -4.1767589569e+01,
  -4.1725292206e+01, -4.1728950500e+01, -4.1741573334e+01, -4.1726192474e+01,
  -4.3251274109e+01, -4.3269790649e+01, -4.3286899567e+01, -4.3264053345e+01,
  -4.3259849548e+01, -4.3289730072e+01, -4.3312843323e+01, -4.3306392670e+01,
  -4.3296531677e+01, -4.3313156128e+01, -4.3292438507e+01, -4.3316799164e+01,
  -4.3337398529e+01, -4.3334651947e+01, -4.3296092987e+01, -4.3342803955e+01,
  -4.3319377899e+01, -4.3273696899e+01, -4.3309402466e+01, -4.3308071136e+01,
  -4.3278896332e+01, -4.3307983398e+01, -4.3313888550e+01, -4.3302410126e+01,
  -4.3289691925e+01, -4.3285034180e+01, -4.3276851654e+01, -4.3305183411e+01,
  -4.3250499725e+01, -4.3262607574e+01, -4.3267635345e+01, -4.3249446869e+01,
  -4.4674316406e+01, -4.4698898315e+01, -4.4700412750e+01, -4.4713195801e+01,
  -4.4697277069e+01, -4.4727066040e+01, -4.4749443054e+01, -4.4728069305e+01,
  -4.4710510254e+01, -4.4708179474e+01, -4.4706680298e+01, -4.4725795746e+01,
  -4.4752220154e+01, -4.4741992950e+01, -4.4724487305e+01, -4.4745670319e+01,
  -4.4735355377e+01, -4.4702064514e+01, -4.4722469330e+01, -4.4723957062e+01,
  -4.4711643219e+01, -4.4702648163e+01, -4.4724624634e+01, -4.4724826813e+01,
  -4.4710201263e+01, -4.4716205597e+01, -4.4718067169e+01, -4.4726787567e+01,
  -4.4688705444e+01, -4.4696422577e+01, -4.4694049835e+01, -4.4676895142e+01,
  -4.5921028137e+01, -4.5951740265e+01, -4.5954433441e+01, -4.5985019684e+01,
  -4.5967987061e+01, -4.5975948334e+01, -4.6001678467e+01, -4.5987499237e+01,
  -4.5970699310e+01, -4.5971847534e+01, -4.5966793060e+01, -4.5972782135e+01,
  -4.6004848480e+01, -4.5998794556e+01, -4.5996322632e+01, -4.6001396179e+01,
  -4.5979808807e+01, -4.5965118408e+01, -4.5989067078e+01, -4.5994174957e+01,
  -4.5974689484e+01, -4.5986457825e+01, -4.5990966797e+01, -4.5986637115e+01,
  -4.5975902557e+01, -4.5964622498e+01, -4.5983108521e+01, -4.5978134155e+01,
  -4.5941249847e+01, -4.5943725586e+01, -4.5924472809e+01, -4.5924167633e+01,
  -4.7422252655e+01, -4.7444396973e+01, -4.7448646545e+01, -4.7469707489e+01,
  -4.7462882996e+01, -4.7469581604e+01, -4.7497112274e+01, -4.7492294312e+01,
  -4.7466850281e+01, -4.7481159210e+01, -4.7472118378e+01, -4.7476486206e+01,
  -4.7488533020e+01, -4.7492687225e+01, -4.7477378845e+01, -4.7499061584e+01,
  -4.7466159821e+01, -4.7446918488e+01, -4.7486652374e+01, -4.7494361877e+01,
  -4.7468204498e+01, -4.7487743378e+01, -4.7505661011e+01, -4.7484252930e+01,
  -4.7462726593e+01, -4.7468944550e+01, -4.7476390839e+01, -4.7469062805e+01,
  -4.7424358368e+01, -4.7431346893e+01, -4.7426620483e+01, -4.7417762756e+01,
  -4.9436561584e+01, -4.9445510864e+01, -4.9478755951e+01, -4.9488151550e+01,
  -4.9486488342e+01, -4.9489448547e+01, -4.9522495270e+01, -4.9506649017e+01,
  -4.9499820709e+01, -4.9504539490e+01, -4.9516551971e+01, -4.9517486572e+01,
  -4.9514331818e+01, -4.9525192261e+01, -4.9488090515e+01, -4.9525093079e+01,
  -4.9502349854e+01, -4.9477096558e+01, -4.9504013062e+01, -4.9524089813e+01,
  -4.9498180389e+01, -4.9527889252e+01, -4.9546619415e+01, -4.9515064240e+01,
  -4.9484706879e+01, -4.9477676392e+01, -4.9483516693e+01, -4.9478916168e+01,
  -4.9437870026e+01, -4.9437599182e+01, -4.9449726105e+01, -4.9442760468e+01,
  -5.1868274689e+01, -5.1870914459e+01, -5.1882450104e+01, -5.1908809662e+01,
  -5.1900096893e+01, -5.1904319763e+01, -5.1926975250e+01, -5.1904232025e+01,
  -5.1901725769e+01, -5.1904407501e+01, -5.1907463074e+01, -5.1914943695e+01,
  -5.1925735474e+01, -5.1922145844e+01, -5.1905647278e+01, -5.1940635681e+01,
  -5.1914085388e+01, -5.1901340485e+01, -5.1902236938e+01, -5.1929450989e+01,
  -5.1916934967e+01, -5.1944023132e+01, -5.1937789917e+01, -5.1927577972e+01,
  -5.1899898529e+01, -5.1893531799e+01, -5.1899917603e+01, -5.1888599396e+01,
  -5.1875789642e+01, -5.1860774994e+01, -5.1865341187e+01, -5.1849876404e+01,
  -5.4004878998e+01, -5.4007266998e+01, -5.4028854370e+01, -5.4046657562e+01,
  -5.4031963348e+01, -5.4040054321e+01, -5.4058269501e+01, -5.4040874481e+01,
  -5.4027629852e+01, -5.4041633606e+01, -5.4045982361e+01, -5.4038516998e+01,
  -5.4049831390e+01, -5.4047805786e+01, -5.4043338776e+01, -5.4068813324e+01,
  -5.4045551300e+01, -5.4022697449e+01, -5.4033603668e+01, -5.4051635742e+01,
  -5.4043811798e+01, -5.4060459137e+01, -5.4068889618e+01, -5.4046470642e+01,
  -5.4032333374e+01, -5.4012336731e+01, -5.4022148132e+01, -5.4006427765e+01,
  -5.3992233276e+01, -5.4015357971e+01, -5.4013431549e+01, -5.3976875305e+01,
  -5.6001640320e+01, -5.6000808716e+01, -5.6021072388e+01, -5.6024383545e+01,
  -5.6021194458e+01, -5.6013763428e+01, -5.6026393890e+01, -5.6018505096e+01,
  -5.6006679535e+01, -5.6023864746e+01, -5.6035598755e+01, -5.6024593353e+01,
  -5.6023399353e+01, -5.6031536102e+01, -5.6018405914e+01, -5.6038520813e+01,
  -5.6033157349e+01, -5.5986110687e+01, -5.6022563934e+01, -5.6026760101e+01,
  -5.6029510498e+01, -5.6031574249e+01, -5.6053558350e+01, -5.6028030396e+01,
  -5.6007118225e+01, -5.5990890503e+01, -5.6003932953e+01, -5.5995983124e+01,
  -5.5973587036e+01, -5.6007114410e+01, -5.5988677979e+01, -5.5976604462e+01,
  -5.7476074219e+01, -5.7481235504e+01, -5.7491832733e+01, -5.7502712250e+01,
  -5.7498241425e+01, -5.7488616943e+01, -5.7507884979e+01, -5.7499309540e+01,
  -5.7496982574e+01, -5.7511238098e+01, -5.7516750336e+01, -5.7508651733e+01,
  -5.7504173279e+01, -5.7499210358e+01, -5.7485252380e+01, -5.7500968933e+01,
  -5.7491596222e+01, -5.7458057404e+01, -5.7482379913e+01, -5.7500923157e+01,
  -5.7500495911e+01, -5.7499412537e+01, -5.7509815216e+01, -5.7485298157e+01,
  -5.7478488922e+01, -5.7481735229e+01, -5.7476707458e+01, -5.7467002869e+01,
  -5.7440174103e+01, -5.7481990814e+01, -5.7478096008e+01, -5.7470577240e+01,
  -5.8458232880e+01, -5.8467346191e+01, -5.8477989197e+01, -5.8489616394e+01,
  -5.8470378876e+01, -5.8469459534e+01, -5.8487960815e+01, -5.8482852936e+01,
  -5.8481414795e+01, -5.8493762970e+01, -5.8486045837e+01, -5.8474716187e+01,
  -5.8473499298e+01, -5.8472606659e+01, -5.8462558746e+01, -5.8485767365e+01,
  -5.8474506378e+01, -5.8462940216e+01, -5.8468593597e+01, -5.8478942871e+01,
  -5.8477310181e+01, -5.8459140778e+01, -5.8477050781e+01, -5.8457099915e+01,
  -5.8450611115e+01, -5.8448871613e+01, -5.8453742981e+01, -5.8439422607e+01,
  -5.8421680450e+01, -5.8464176178e+01, -5.8458396912e+01, -5.8443904877e+01,
  -5.9409832001e+01, -5.9405471802e+01, -5.9418033600e+01, -5.9437789917e+01,
  -5.9420455933e+01, -5.9417575836e+01, -5.9431510925e+01, -5.9436069489e+01,
  -5.9427753448e+01, -5.9436271667e+01, -5.9441509247e+01, -5.9428710938e+01,
  -5.9415546417e+01, -5.9402465820e+01, -5.9404838562e+01, -5.9422016144e+01,
  -5.9428081512e+01, -5.9427700043e+01, -5.9419178009e+01, -5.9420372009e+01,
  -5.9417812347e+01, -5.9406002045e+01, -5.9418956757e+01, -5.9397159576e+01,
  -5.9396789551e+01, -5.9392150879e+01, -5.9401592255e+01, -5.9389636993e+01,
  -5.9389720917e+01, -5.9405597687e+01, -5.9413425446e+01, -5.9410182953e+01
};

const float featureScalerStd[960] = {
  6.3109979630e+00, 6.2908587456e+00, 6.2272028923e+00, 6.2585501671e+00,
  6.2220816612e+00, 6.2059216499e+00, 6.2160387039e+00, 6.1966357231e+00,
  6.2254214287e+00, 6.2131710052e+00, 6.1866378784e+00, 6.1653218269e+00,
  6.1590204239e+00, 6.1780972481e+00, 6.1893472672e+00, 6.1845436096e+00,
  6.1588950157e+00, 6.1592550278e+00, 6.1548585892e+00, 6.1396069527e+00,
  6.1515703201e+00, 6.1607770920e+00, 6.1476459503e+00, 6.1910843849e+00,
  6.2038626671e+00, 6.2160754204e+00, 6.2106919289e+00, 6.2038574219e+00,
  6.1961693764e+00, 6.2279796600e+00, 6.2310924530e+00, 6.2638759613e+00,
  6.5287914276e+00, 6.4934582710e+00, 6.4590921402e+00, 6.4775481224e+00,
  6.4691963196e+00, 6.4311842918e+00, 6.4350852966e+00, 6.4427480698e+00,
  6.4184489250e+00, 6.4052143097e+00, 6.3804216385e+00, 6.3898806572e+00,
  6.3848285675e+00, 6.4176158905e+00, 6.4174160957e+00, 6.3767504692e+00,
  6.3716197014e+00, 6.3550853729e+00, 6.3733267784e+00, 6.3573341370e+00,
  6.3710451126e+00, 6.3722033501e+00, 6.3951911926e+00, 6.4105439186e+00,
  6.4375181198e+00, 6.4315962791e+00, 6.4150366783e+00, 6.4342579842e+00,
  6.4204864502e+00, 6.4362192154e+00, 6.4409270287e+00, 6.4570655823e+00,
  7.6964159012e+00, 7.6971249580e+00, 7.6853861809e+00, 7.6433496475e+00,
  7.6363620758e+00, 7.6414589882e+00, 7.6237335205e+00, 7.6240892410e+00,
  7.6459870338e+00, 7.6271643639e+00, 7.5997858047e+00, 7.5830154419e+00,
  7.6164622307e+00, 7.6216087341e+00, 7.6131272316e+00, 7.5873322487e+00,
  7.5840234756e+00, 7.5836448669e+00, 7.5994277000e+00, 7.5858764648e+00,
  7.5811572075e+00, 7.5749249458e+00, 7.5904045105e+00, 7.6142706871e+00,
  7.6457295418e+00, 7.6419730186e+00, 7.6315898895e+00, 7.5974631310e+00,
  7.6208500862e+00, 7.6018700600e+00, 7.6251153946e+00, 7.6574392319e+00,
  8.7256155014e+00, 8.7134017944e+00, 8.6982107162e+00, 8.6761388779e+00,
  8.6685247421e+00, 8.6699485779e+00, 8.6540451050e+00, 8.6671438217e+00,
  8.6682300568e+00, 8.6281805038e+00, 8.6157073975e+00, 8.6137838364e+00,
  8.6168432236e+00, 8.6267509460e+00, 8.6271076202e+00, 8.6173925400e+00,
  8.6099433899e+00, 8.6041030884e+00, 8.6314973831e+00, 8.5919284821e+00,
  8.6143970490e+00, 8.6033620834e+00, 8.6359300613e+00, 8.6474313736e+00,
  8.6703577042e+00, 8.6476917267e+00, 8.6702308655e+00, 8.6627521515e+00,
  8.6621894836e+00, 8.6571969986e+00, 8.6546669006e+00, 8.6868448257e+00,
  9.6141328812e+00, 9.5868034363e+00, 9.5721712112e+00, 9.5645151138e+00,
  9.5537939072e+00, 9.5293750763e+00, 9.5401515961e+00, 9.5366268158e+00,
  9.5219926834e+00, 9.5182981491e+00, 9.5220508575e+00, 9.5159282684e+00,
  9.5130062103e+00, 9.5157175064e+00, 9.5097351074e+00, 9.4802026749e+00,
  9.4981002808e+00, 9.5124740601e+00, 9.4877538681e+00, 9.4739694595e+00,
  9.4922370911e+00, 9.4667730331e+00, 9.5016469955e+00, 9.5267524719e+00,
  9.5110826492e+00, 9.5124063492e+00, 9.5148010254e+00, 9.5102138519e+00,
  9.5048990250e+00, 9.5121307373e+00, 9.5123233795e+00, 9.5388517380e+00,
  1.0049070358e+01, 1.0033421516e+01, 1.0024824142e+01, 9.9965839386e+00,
  1.0013217926e+01, 1.0008565903e+01, 9.9870347977e+00, 9.9921169281e+00,
  9.9722871780e+00, 9.9907827377e+00, 9.9773969650e+00, 9.9688177109e+00,
  9.9617176056e+00, 9.9823226929e+00, 9.9753341675e+00, 9.9695835114e+00,
  9.9682950974e+00, 9.9489965439e+00, 9.9252529144e+00, 9.9230623245e+00,
  9.9309949875e+00, 9.9203548431e+00, 9.9423770905e+00, 9.9505653381e+00,
  9.9600296021e+00, 9.9750051498e+00, 9.9578924179e+00, 9.9347743988e+00,
  9.9357595444e+00, 9.9654903412e+00, 9.9805879593e+00, 1.0006041527e+01,
  1.0149080276e+01, 1.0148346901e+01, 1.0116251945e+01, 1.0069352150e+01,
  1.0085348129e+01, 1.0090450287e+01, 1.0075937271e+01, 1.0042883873e+01,
  1.0050775528e+01, 1.0073410988e+01, 1.0043160439e+01, 1.0052380562e+01,
  1.0074896812e+01, 1.0068876266e+01, 1.0054901123e+01, 1.0048597336e+01,
  1.0052879333e+01, 1.0016428947e+01, 9.9993686676e+00, 1.0020141602e+01,
  1.0030133247e+01, 1.0013991356e+01, 1.0036190987e+01, 1.0048510551e+01,
  1.0037520409e+01, 1.0060993195e+01, 1.0062909126e+01, 1.0052195549e+01,
  1.0062154770e+01, 1.0077612877e+01, 1.0099896431e+01, 1.0108371735e+01,
  1.0188390732e+01, 1.0148090363e+01, 1.0138211250e+01, 1.0099186897e+01,
  1.0102682114e+01, 1.0103007317e+01, 1.0078450203e+01, 1.0044148445e+01,
  1.0066473007e+01, 1.0067728043e+01, 1.0071689606e+01, 1.0072333336e+01,
  1.0059342384e+01, 1.0060923576e+01, 1.0073431969e+01, 1.0081758499e+01,
  1.0068238258e+01, 1.0047307014e+01, 1.0049679756e+01, 1.0065538406e+01,
  1.0054857254e+01, 1.0039834976e+01, 1.0033417702e+01, 1.0055044174e+01,
  1.0043432236e+01, 1.0053409576e+01, 1.0092086792e+01, 1.0071458817e+01,
  1.0121054649e+01, 1.0104925156e+01, 1.0096126556e+01, 1.0133932114e+01,
  1.0344996452e+01, 1.0326523781e+01, 1.0301461220e+01, 1.0282618523e+01,
  1.0306664467e+01, 1.0295655251e+01, 1.0275780678e+01, 1.0235122681e+01,
  1.0247987747e+01, 1.0252130508e+01, 1.0252393723e+01, 1.0256160736e+01,
  1.0260446548e+01, 1.0272611618e+01, 1.0275405884e+01, 1.0273730278e+01,
  1.0266011238e+01, 1.0267452240e+01, 1.0257193565e+01, 1.0256621361e+01,
  1.0254320145e+01, 1.0246506691e+01, 1.0251341820e+01, 1.0260262489e+01,
  1.0262785912e+01, 1.0262650490e+01, 1.0276261330e+01, 1.0281044006e+01,
  1.0316746712e+01, 1.0297070503e+01, 1.0290446281e+01, 1.0306355476e+01,
  1.0511736870e+01, 1.0492448807e+01, 1.0497115135e+01, 1.0478238106e+01,
  1.0455080986e+01, 1.0444006920e+01, 1.0438695908e+01, 1.0438561440e+01,
  1.0435740471e+01, 1.0450082779e+01, 1.0427315712e+01, 1.0445605278e+01,
  1.0463839531e+01, 1.0453577042e+01, 1.0453330040e+01, 1.0474383354e+01,
  1.0454660416e+01, 1.0438715935e+01, 1.0420905113e+01, 1.0396602631e+01,
  1.0431228638e+01, 1.0443230629e+01, 1.0446114540e+01, 1.0450003624e+01,
  1.0399106979e+01, 1.0397089005e+01, 1.0418314934e+01, 1.0436402321e+01,
  1.0449459076e+01, 1.0459421158e+01, 1.0467018127e+01, 1.0463256836e+01,
  1.0662684441e+01, 1.0648088455e+01, 1.0612134933e+01, 1.0606942177e+01,
  1.0598278046e+01, 1.0593487740e+01, 1.0583276749e+01, 1.0590166092e+01,
  1.0563846588e+01, 1.0572052002e+01, 1.0588653564e+01, 1.0573555946e+01,
  1.0594039917e+01, 1.0571977615e+01, 1.0594381332e+01, 1.0622845650e+01,
  1.0600781441e+01, 1.0577335358e+01, 1.0565224648e+01, 1.0524238586e+01,
  1.0564267159e+01, 1.0556104660e+01, 1.0581970215e+01, 1.0586555481e+01,
  1.0542935371e+01, 1.0529734612e+01, 1.0582606316e+01, 1.0578113556e+01,
  1.0581613541e+01, 1.0596154213e+01, 1.0607628822e+01, 1.0608695030e+01,
  1.1042004585e+01, 1.1006828308e+01, 1.0967871666e+01, 1.0939256668e+01,
  1.0958163261e+01, 1.0951064110e+01, 1.0924144745e+01, 1.0905205727e+01,
  1.0894048691e+01, 1.0887475014e+01, 1.0905559540e+01, 1.0908518791e+01,
  1.0908629417e+01, 1.0895954132e+01, 1.0930022240e+01, 1.0930339813e+01,
  1.0920058250e+01, 1.0917044640e+01, 1.0911455154e+01, 1.0901865005e+01,
  1.0900545120e+01, 1.0905197144e+01, 1.0922256470e+01, 1.0921905518e+01,
  1.0888095856e+01, 1.0914608955e+01, 1.0931364059e+01, 1.0922201157e+01,
  1.0931862831e+01, 1.0956911087e+01, 1.0952315331e+01, 1.0958271980e+01,
  1.1260467529e+01, 1.1226020813e+01, 1.1194004059e+01, 1.1168116570e+01,
  1.1166927338e+01, 1.1172461510e+01, 1.1173640251e+01, 1.1144491196e+01,
  1.1129194260e+01, 1.1112619400e+01, 1.1161342621e+01, 1.1164002419e+01,
  1.1158647537e+01, 1.1178745270e+01, 1.1141294479e+01, 1.1155679703e+01,
  1.1172365189e+01, 1.1164878845e+01, 1.1134952545e+01, 1.1117042542e+01,
  1.1133545876e+01, 1.1139767647e+01, 1.1149160385e+01, 1.1148824692e+01,
  1.1141765594e+01, 1.1123301506e+01, 1.1134674072e+01, 1.1142917633e+01,
  1.1153030396e+01, 1.1188525200e+01, 1.1179353714e+01, 1.1214090347e+01,
  1.1670560837e+01, 1.1638636589e+01, 1.1637295723e+01, 1.1633964539e+01,
  1.1607422829e+01, 1.1599185944e+01, 1.1600179672e+01, 1.1575470924e+01,
  1.1566156387e+01, 1.1564376831e+01, 1.1601095200e+01, 1.1610156059e+01,
  1.1614181519e+01, 1.1616724014e+01, 1.1602370262e+01, 1.1605874062e+01,
  1.1612834930e+01, 1.1606542587e+01, 1.1579649925e+01, 1.1546530724e+01,
  1.1562105179e+01, 1.1554540634e+01, 1.1559415817e+01, 1.1575285912e+01,
  1.1579356194e+01, 1.1565052986e+01, 1.1549180984e+01, 1.1558725357e+01,
  1.1590943336e+01, 1.1595332146e+01, 1.1616847038e+01, 1.1626115799e+01,
  1.1942794800e+01, 1.1931732178e+01, 1.1924601555e+01, 1.1889503479e+01,
  1.1896960258e+01, 1.1887652397e+01, 1.1871484756e+01, 1.1864366531e+01,
  1.1849719048e+01, 1.1864952087e+01, 1.1858719826e+01, 1.1876677513e+01,
  1.1890563965e+01, 1.1894374847e+01, 1.1896147728e+01, 1.1893488884e+01,
  1.1886159897e+01, 1.1868949890e+01, 1.1866863251e+01, 1.1840703011e+01,
  1.1832215309e+01, 1.1841880798e+01, 1.1828715324e+01, 1.1850297928e+01,
  1.1866333008e+01, 1.1836732864e+01, 1.1850098610e+01, 1.1853891373e+01,
  1.1855887413e+01, 1.1883165359e+01, 1.1915002823e+01, 1.1913589478e+01,
  1.1907999039e+01, 1.1883720398e+01, 1.1853745461e+01, 1.1841197968e+01,
  1.1833512306e+01, 1.1824970245e+01, 1.1816617966e+01, 1.1799597740e+01,
  1.1785913467e+01, 1.1820315361e+01, 1.1810605049e+01, 1.1824269295e+01,
  1.1836957932e+01, 1.1827518463e+01, 1.1839057922e+01, 1.1826335907e+01,
  1.1810332298e+01, 1.1783105850e+01, 1.1788965225e+01, 1.1783069611e+01,
  1.1762825966e+01, 1.1734070778e+01, 1.1758977890e+01, 1.1765296936e+01,
  1.1799095154e+01, 1.1781395912e+01, 1.1802581787e+01, 1.1814729691e+01,
  1.1827819824e+01, 1.1822568893e+01, 1.1858787537e+01, 1.1861707687e+01,
  1.2057948112e+01, 1.2020170212e+01, 1.1994922638e+01, 1.2000727654e+01,
  1.1991347313e+01, 1.1982773781e+01, 1.1986082077e+01, 1.1963343620e+01,
  1.1964024544e+01, 1.1977493286e+01, 1.1993801117e+01, 1.1999196053e+01,
  1.1995484352e+01, 1.2002439499e+01, 1.2012715340e+01, 1.1989794731e+01,
  1.1966835976e+01, 1.1980634689e+01, 1.1968950272e+01, 1.1959680557e+01,
  1.1949012756e+01, 1.1942746162e+01, 1.1957008362e+01, 1.1960164070e+01,
  1.1970336914e+01, 1.1943821907e+01, 1.1973557472e+01, 1.2002706528e+01,
  1.2017164230e+01, 1.2024147987e+01, 1.2016323090e+01, 1.2032241821e+01,
  1.2132282257e+01, 1.2110263824e+01, 1.2096486092e+01, 1.2078269958e+01,
  1.2069705963e+01, 1.2065425873e+01, 1.2058526993e+01, 1.2040797234e+01,
  1.2033662796e+01, 1.2044915199e+01, 1.2072670937e+01, 1.2075208664e+01,
  1.2070290565e+01, 1.2084679604e+01, 1.2104640961e+01, 1.2078166962e+01,
  1.2087986946e+01, 1.2081668854e+01, 1.2052600861e+01, 1.2046431541e+01,
  1.2046447754e+01, 1.2061999321e+01, 1.2041502953e+01, 1.2046717644e+01,
  1.2056054115e+01, 1.2058556557e+01, 1.2077600479e+01, 1.2095165253e+01,
  1.2100222588e+01, 1.2093366623e+01, 1.2116413116e+01, 1.2113665581e+01,
  1.2315494537e+01, 1.2312620163e+01, 1.2292950630e+01, 1.2273590088e+01,
  1.2272086143e+01, 1.2263301849e+01, 1.2260104179e+01, 1.2238539696e+01,
  1.2234725952e+01, 1.2228268623e+01, 1.2236508369e+01, 1.2245103836e+01,
  1.2262061119e+01, 1.2284091949e+01, 1.2285748482e+01, 1.2262106895e+01,
  1.2268354416e+01, 1.2243217468e+01, 1.2258559227e+01, 1.2231879234e+01,
  1.2254099846e+01, 1.2257054329e+01, 1.2248468399e+01, 1.2248544693e+01,
  1.2267435074e+01, 1.2258700371e+01, 1.2273361206e+01, 1.2279485703e+01,
  1.2292826653e+01, 1.2290529251e+01, 1.2307523727e+01, 1.2305631638e+01,
  1.2428253174e+01, 1.2408769608e+01, 1.2399991989e+01, 1.2361321449e+01,
  1.2361578941e+01, 1.2340999603e+01, 1.2340888977e+01, 1.2307427406e+01,
  1.2307931900e+01, 1.2324439049e+01, 1.2339303017e+01, 1.2355308533e+01,
  1.2359992981e+01, 1.2387964249e+01, 1.2376611710e+01, 1.2348369598e+01,
  1.2358726501e+01, 1.2347538948e+01, 1.2360380173e+01, 1.2347792625e+01,
  1.2380926132e+01, 1.2364876747e+01, 1.2351462364e+01, 1.2357096672e+01,
  1.2369817734e+01, 1.2346028328e+01, 1.2364645958e+01, 1.2390392303e+01,
  1.2391607285e+01, 1.2396087646e+01, 1.2396889687e+01, 1.2395752907e+01,
  1.2483848572e+01, 1.2469942093e+01, 1.2442683220e+01, 1.2406536102e+01,
  1.2410554886e+01, 1.2408399582e+01, 1.2384711266e+01, 1.2391265869e+01,
  1.2403086662e+01, 1.2416207314e+01, 1.2433396339e+01, 1.2434637070e+01,
  1.2440582275e+01, 1.2462311745e+01, 1.2469914436e+01, 1.2428209305e+01,
  1.2422031403e+01, 1.2417624474e+01, 1.2417043686e+01, 1.2413372993e+01,
  1.2438095093e+01, 1.2440349579e+01, 1.2436028481e+01, 1.2448474884e+01,
  1.2459209442e+01, 1.2433026314e+01, 1.2445264816e+01, 1.2464260101e+01,
  1.2474756241e+01, 1.2452718735e+01, 1.2454924583e+01, 1.2475885391e+01,
  1.2350564003e+01, 1.2332379341e+01, 1.2295842171e+01, 1.2259199142e+01,
  1.2249415398e+01, 1.2268890381e+01, 1.2247097969e+01, 1.2255354881e+01,
  1.2257320404e+01, 1.2272824287e+01, 1.2307569504e+01, 1.2311685562e+01,
  1.2305744171e+01, 1.2324080467e+01, 1.2317101479e+01, 1.2301108360e+01,
  1.2285813332e+01, 1.2262001991e+01, 1.2260416031e+01, 1.2263674736e+01,
  1.2268937111e+01, 1.2268498421e+01, 1.2299918175e+01, 1.2301256180e+01,
  1.2304275513e+01, 1.2285023689e+01, 1.2306157112e+01, 1.2322032928e+01,
  1.2336982727e+01, 1.2305367470e+01, 1.2297709465e+01, 1.2336209297e+01,
  1.2390614510e+01, 1.2362256050e+01, 1.2334664345e+01, 1.2310378075e+01,
  1.2304215431e+01, 1.2309120178e+01, 1.2280812263e+01, 1.2283642769e+01,
  1.2292198181e+01, 1.2286880493e+01, 1.2316241264e+01, 1.2336452484e+01,
  1.2339106560e+01, 1.2342276573e+01, 1.2346446991e+01, 1.2341824532e+01,
  1.2310288429e+01, 1.2278606415e+01, 1.2296405792e+01, 1.2281836510e+01,
  1.2292894363e+01, 1.2283602715e+01, 1.2319580078e+01, 1.2320269585e+01,
  1.2325643539e+01, 1.2321735382e+01, 1.2338781357e+01, 1.2355959892e+01,
  1.2354413033e+01, 1.2326824188e+01, 1.2352916718e+01, 1.2382012367e+01,
  1.2299120903e+01, 1.2260744095e+01, 1.2250806808e+01, 1.2236295700e+01,
  1.2214118958e+01, 1.2221914291e+01, 1.2204668045e+01, 1.2188223839e+01,
  1.2202225685e+01, 1.2212316513e+01, 1.2238094330e+01, 1.2257822037e+01,
  1.2262115479e+01, 1.2265620232e+01, 1.2283443451e+01, 1.2284039497e+01,
  1.2242465973e+01, 1.2216855049e+01, 1.2214548111e+01, 1.2197027206e+01,
  1.2231816292e+01, 1.2211387634e+01, 1.2226497650e+01, 1.2219143867e+01,
  1.2249870300e+01, 1.2257699966e+01, 1.2275739670e+01, 1.2284532547e+01,
  1.2270982742e+01, 1.2231842995e+01, 1.2266588211e+01, 1.2299262047e+01,
  1.2533109665e+01, 1.2493188858e+01, 1.2487019539e+01, 1.2474425316e+01,
  1.2469799995e+01, 1.2464872360e+01, 1.2465846062e+01, 1.2433547974e+01,
  1.2445105553e+01, 1.2474591255e+01, 1.2488312721e+01, 1.2496947289e+01,
  1.2508327484e+01, 1.2517092705e+01, 1.2533761024e+01, 1.2532264709e+01,
  1.2488808632e+01, 1.2455943108e+01, 1.2462852478e+01, 1.2447892189e+01,
  1.2476842880e+01, 1.2466765404e+01, 1.2471072197e+01, 1.2467674255e+01,
  1.2482236862e+01, 1.2486462593e+01, 1.2503142357e+01, 1.2499393463e+01,
  1.2483269691e+01, 1.2467063904e+01, 1.2502334595e+01, 1.2516973495e+01,
  1.2876175880e+01, 1.2838794708e+01, 1.2828380585e+01, 1.2821989059e+01,
  1.2817567825e+01, 1.2824381828e+01, 1.2809131622e+01, 1.2792384148e+01,
  1.2798399925e+01, 1.2799511909e+01, 1.2813722610e+01, 1.2823854446e+01,
  1.2843356133e+01, 1.2860274315e+01, 1.2876405716e+01, 1.2888090134e+01,
  1.2852269173e+01, 1.2823070526e+01, 1.2828790665e+01, 1.2799398422e+01,
  1.2830722809e+01, 1.2825596809e+01, 1.2823852539e+01, 1.2832250595e+01,
  1.2836755753e+01, 1.2837595940e+01, 1.2836318970e+01, 1.2842752457e+01,
  1.2829253197e+01, 1.2831872940e+01, 1.2856936455e+01, 1.2880241394e+01,
  1.2952093124e+01, 1.2941888809e+01, 1.2919299126e+01, 1.2918901443e+01,
  1.2934229851e+01, 1.2917593956e+01, 1.2901948929e+01, 1.2887329102e+01,
  1.2887516975e+01, 1.2878761292e+01, 1.2906949043e+01, 1.2929411888e+01,
  1.2925868988e+01, 1.2948934555e+01, 1.2968623161e+01, 1.2964770317e+01,
  1.2936423302e+01, 1.2923710823e+01, 1.2915297508e+01, 1.2880368233e+01,
  1.2896382332e+01, 1.2909051895e+01, 1.2918786049e+01, 1.2924029350e+01,
  1.2938171387e+01, 1.2922247887e+01, 1.2917959213e+01, 1.2929511070e+01,
  1.2926914215e+01, 1.2923849106e+01, 1.2946648598e+01, 1.2962222099e+01,
  1.2950118065e+01, 1.2920932770e+01, 1.2923212051e+01, 1.2906203270e+01,
  1.2920022011e+01, 1.2903882980e+01, 1.2898805618e+01, 1.2892223358e+01,
  1.2883786201e+01, 1.2868007660e+01, 1.2905918121e+01, 1.2924333572e+01,
  1.2918680191e+01, 1.2942454338e+01, 1.2950670242e+01, 1.2944532394e+01,
  1.2915154457e+01, 1.2906334877e+01, 1.2916740417e+01, 1.2877485275e+01,
  1.2883134842e+01, 1.2908996582e+01, 1.2923642159e+01, 1.2911266327e+01,
  1.2931886673e+01, 1.2898125648e+01, 1.2900618553e+01, 1.2909650803e+01,
  1.2913376808e+01, 1.2906631470e+01, 1.2932274818e+01, 1.2944234848e+01,
  1.3014129639e+01, 1.2980032921e+01, 1.2976831436e+01, 1.2968846321e+01,
  1.2978304863e+01, 1.2965986252e+01, 1.2966539383e+01, 1.2965028763e+01,
  1.2966432571e+01, 1.2952330589e+01, 1.2984536171e+01, 1.2987082481e+01,
  1.2982834816e+01, 1.3008023262e+01, 1.3013829231e+01, 1.3004839897e+01,
  1.2983256340e+01, 1.2980598450e+01, 1.2987730980e+01, 1.2952786446e+01,
  1.2957074165e+01, 1.2961975098e+01, 1.2984695435e+01, 1.2979676247e+01,
  1.2989139557e+01, 1.2970261574e+01, 1.2979083061e+01, 1.2987648964e+01,
  1.2978797913e+01, 1.2967457771e+01, 1.2983685493e+01, 1.3000646591e+01,
  1.2975831985e+01, 1.2951325417e+01, 1.2952245712e+01, 1.2951297760e+01,
  1.2952430725e+01, 1.2943903923e+01, 1.2947068214e+01, 1.2953403473e+01,
  1.2961033821e+01, 1.2951451302e+01, 1.2962648392e+01, 1.2957874298e+01,
  1.2961213112e+01, 1.2976863861e+01, 1.2985722542e+01, 1.2986889839e+01,
  1.2958802223e+01, 1.2954181671e+01, 1.2955447197e+01, 1.2934936523e+01,
  1.2937433243e+01, 1.2938313484e+01, 1.2954934120e+01, 1.2949970245e+01,
  1.2960661888e+01, 1.2946292877e+01, 1.2961439133e+01, 1.2960653305e+01,
  1.2953073502e+01, 1.2943588257e+01, 1.2941719055e+01, 1.2966838837e+01
};
