namespace web.admin
{
    using System;
    using System.Web.UI;
    using System.Web.UI.WebControls;
    using web;

    public class cpfunset : Page
    {
        protected Button btnedit;
        protected TextBox tbcharesetcount;
        protected TextBox tbcharesetflv;
        protected TextBox tbcharesetfmoney;
        protected TextBox tbcharesetgivepoint;
        protected TextBox tbcharesetlvstep;
        protected TextBox tbcharesetmoneystep;
        protected TextBox tbclearpkgold;
        protected TextBox tbcommendgold;
        protected TextBox tbcommendmoney;
        protected TextBox tbcommendtext;
        protected TextBox tbgoldtomoney;
        protected TextBox tbitemlvupgold;
        protected TextBox tbitemlvuprate;
        protected TextBox tbmodifychanamecount;
        protected TextBox tbmodifychanamemoney;
        protected TextBox tbpetlvupgold2;
        protected TextBox tbpetlvupgold3;
        protected TextBox tbpssigngold;
        protected TextBox tbresetpointgold;
        protected TextBox tbresetpointmoney;
        protected TextBox tbskilllvupmoney;
        protected TextBox tbvipoffer;

        private void btnedit_Click(object sender, EventArgs e)
        {
            XmlControl control = new XmlControl(base.Server.MapPath("/config/config.config"));
            control.updateContent("config/game/modifychanamecount", this.tbmodifychanamecount.Text.ToString().Trim());
            control.updateContent("config/game/modifychanamemoney", this.tbmodifychanamemoney.Text.ToString().Trim());
            control.updateContent("config/game/charesetcount", this.tbcharesetcount.Text.ToString().Trim());
            control.updateContent("config/game/charesetgivepoint", this.tbcharesetgivepoint.Text.ToString().Trim());
            control.updateContent("config/game/charesetflv", this.tbcharesetflv.Text.ToString().Trim());
            control.updateContent("config/game/charesetlvstep", this.tbcharesetlvstep.Text.ToString().Trim());
            control.updateContent("config/game/charesetfmoney", this.tbcharesetfmoney.Text.ToString().Trim());
            control.updateContent("config/game/charesetmoneystep", this.tbcharesetmoneystep.Text.ToString().Trim());
            control.updateContent("config/game/commendmoney", this.tbcommendmoney.Text.ToString().Trim());
            control.updateContent("config/game/commendgold", this.tbcommendgold.Text.ToString().Trim());
            control.updateContent("config/game/clearpkgold", this.tbclearpkgold.Text.ToString().Trim());
            control.updateContent("config/game/petlvupgold2", this.tbpetlvupgold2.Text.ToString().Trim());
            control.updateContent("config/game/petlvupgold3", this.tbpetlvupgold3.Text.ToString().Trim());
            control.updateContent("config/game/skilllvupmoney", this.tbskilllvupmoney.Text.ToString().Trim());
            control.updateContent("config/game/goldtomoney", this.tbgoldtomoney.Text.ToString().Trim());
            control.updateContent("config/game/resetpointmoney", this.tbresetpointmoney.Text.ToString().Trim());
            control.updateContent("config/game/resetpointgold", this.tbresetpointgold.Text.ToString().Trim());
            control.updateContent("config/game/vipoffer", this.tbvipoffer.Text.ToString().Trim());
            control.updateContent("config/game/pssigngold", this.tbpssigngold.Text.ToString().Trim());
            control.Save();
            new DataProviders().ExecuteSql("update mhcmember..web_notice set commend='" + new system().ChkSql(this.tbcommendtext.Text.ToString().Trim()) + "'");
            new system().loadConfig(1);
        }

        private void InitializeComponent()
        {
            this.tbcommendgold.TextChanged += new EventHandler(this.tbcommendsp_TextChanged);
            this.btnedit.Click += new EventHandler(this.btnedit_Click);
            base.Load += new EventHandler(this.Page_Load);
        }

        protected override void OnInit(EventArgs e)
        {
            this.InitializeComponent();
            base.OnInit(e);
        }

        private void Page_Load(object sender, EventArgs e)
        {
            new WebLogic().isadmin();
            new WebLogic().diskf();
            new system().loadConfig(1);
            if (!this.Page.IsPostBack)
            {
                this.tbmodifychanamecount.Text = base.Application["game.modifychanamecount"].ToString();
                this.tbmodifychanamemoney.Text = base.Application["game.modifychanamemoney"].ToString();
                this.tbcharesetcount.Text = base.Application["game.charesetcount"].ToString();
                this.tbcharesetgivepoint.Text = base.Application["game.charesetgivepoint"].ToString();
                this.tbcharesetflv.Text = base.Application["game.charesetflv"].ToString();
                this.tbcharesetlvstep.Text = base.Application["game.charesetlvstep"].ToString();
                this.tbcharesetfmoney.Text = base.Application["game.charesetfmoney"].ToString();
                this.tbcharesetmoneystep.Text = base.Application["game.charesetmoneystep"].ToString();
                this.tbcommendmoney.Text = base.Application["game.commendmoney"].ToString();
                this.tbcommendgold.Text = base.Application["game.commendgold"].ToString();
                this.tbclearpkgold.Text = base.Application["game.clearpkgold"].ToString();
                this.tbpetlvupgold2.Text = base.Application["game.petlvupgold2"].ToString();
                this.tbpetlvupgold3.Text = base.Application["game.petlvupgold3"].ToString();
                this.tbskilllvupmoney.Text = base.Application["game.skilllvupmoney"].ToString();
                this.tbgoldtomoney.Text = base.Application["game.goldtomoney"].ToString();
                this.tbvipoffer.Text = base.Application["game.vipoffer"].ToString();
                this.tbcommendtext.Text = base.Application["commendtext"].ToString();
                this.tbresetpointmoney.Text = base.Application["game.resetpointmoney"].ToString();
                this.tbresetpointgold.Text = base.Application["game.resetpointgold"].ToString();
                this.tbpssigngold.Text = base.Application["game.pssigngold"].ToString();
            }
        }

        private void tbcommendsp_TextChanged(object sender, EventArgs e)
        {
        }
    }
}
